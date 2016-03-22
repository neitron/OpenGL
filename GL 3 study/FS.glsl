#version 330          
                
const int MAX_POINT_LIGHTS  = 2;
const int MAX_SPOT_LIGHTS   = 2;

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;
in vec3 tangent0;

in vec4 lightSpacePos;

out vec4 fragColor;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Attenuation                                                             
{
  float constant;
  float linear;
  float expVar;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BaseLight
{
  float diffuseIntensity;
  float ambientIntensity;
  vec3  color;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct DirectionalLight                                                             
{
  BaseLight   base;
  vec3        direction;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PointLight                                                             
{
  BaseLight     base;
  Attenuation   atten;
  vec3          position;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct SpotLight
{
  PointLight  base;
  vec3        direction;
  float       cutOff;    // cos of angle
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
uniform SpotLight   gSpotLights [ MAX_SPOT_LIGHTS ];
uniform int         gNumSpotLights;

uniform PointLight			  gPointLights [ MAX_POINT_LIGHTS ];
uniform int					      gNumPointLights;

uniform DirectionalLight	gDirectionalLight;

uniform sampler2D			gColorMap;
uniform sampler2D			gShadowMap;
uniform sampler2D     gNormalMap;

uniform vec3				gEyeWorldPos;
uniform float				gMatSpecularIntensity;
uniform float				gSpecularPower;

uniform int   gIsUseNormalMap;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calc normal from normalMap to 
vec3 CalcBumpedNormal ( )
{
  vec3 normal  = normalize ( normal0 );
  if ( gIsUseNormalMap == 0 )
  { 
    return  normal; 
  }
                                   
  vec3 tangent = normalize ( tangent0 );       

  tangent = normalize ( tangent - dot ( tangent, normal ) * normal );                 
  
  vec3 bitangent = cross ( tangent, normal );                                         
  vec3 bumpMapNormal = texture ( gNormalMap, texCoord0 ).xyz;
  bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);

  mat3 tbn = mat3 ( tangent, bitangent, normal);
           
  return 
    normalize ( tbn * bumpMapNormal );
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float CalcShadowFactor(vec4 lightSpacePos)
{
  // Деление проекции (перспективы) 
  // WVP (Clip Space) => нормированные координаты NDC 
  // (не путать с простой нормализацией)
  vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w; 
  vec2 UVCoords;
  
  // Мы переводим координаты NDC (xMin = -1, xMax = 1) 
  // в тестурные (xMin = 0, xMax = 1)
	UVCoords.x = 0.5 * projCoords.x + 0.5;
  UVCoords.y = 0.5 * projCoords.y + 0.5;
  
  float depth = texture ( gShadowMap, UVCoords ).x;
    
	if ( depth <= ( projCoords.z + 0.005 ) )
	{
    return 0.2;
	}
  else
	{
    return 1.0;
	}
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcLightInternal ( BaseLight light, vec3 lightDirection, vec3 normal, float shadowFactor )
{
  vec4	ambientColor	= vec4 ( light.color, 1.0f ) * light.ambientIntensity;
  vec4  diffuseColor  = vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
  vec4  specularColor = vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
  
  float	diffuseFactor	= dot ( normal, -lightDirection );

  if ( diffuseFactor > 0.0f ) 
  {
    diffuseColor = vec4 ( light.color, 1.0f ) * light.diffuseIntensity * diffuseFactor;

    vec3  vertexToEye     = normalize ( gEyeWorldPos - worldPos0 );
    vec3  lightReflect    = normalize ( reflect ( lightDirection, normal ) ); // get normalize reflect light vector
    float specularFactor  = dot ( vertexToEye, lightReflect );

    specularFactor = pow ( specularFactor, gSpecularPower );

    if ( specularFactor > 0.0f ) 
    {
      specularColor = vec4 ( light.color, 1.0f ) * gMatSpecularIntensity * specularFactor;
    }
  }

  return ( ambientColor + shadowFactor * ( diffuseColor + specularColor ) );
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcDirectionalLight ( vec3 normal )
{
     return CalcLightInternal ( gDirectionalLight.base, gDirectionalLight.direction, normal, 1.0);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcPointLight ( PointLight pointLight, vec3 normal, vec4 lightSpacePos )
{
  vec3 lightDirection = worldPos0 - pointLight.position;
  float distanceVar = length ( lightDirection );
  lightDirection = normalize ( lightDirection );

  float shadowFactor = CalcShadowFactor ( lightSpacePos );

  vec4 color = CalcLightInternal ( pointLight.base, lightDirection, normal, shadowFactor);
  
  float attenuation =  pointLight.atten.constant +
                       pointLight.atten.linear * distanceVar +
                       pointLight.atten.expVar * distanceVar * distanceVar;
  
  return color / attenuation;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcSpotLight ( SpotLight sLight, vec3 normal, vec4 lightSpacePos )
{
  vec3 lightToPixel = normalize ( worldPos0 - sLight.base.position);
  float spotFactor = dot ( lightToPixel, sLight.direction );
  
  if ( spotFactor > sLight.cutOff ) 
  {
    vec4 color = CalcPointLight ( sLight.base, normal, lightSpacePos);
    
    return 
      color * ( 1.0f - ( 1.0f - spotFactor ) * 1.0f / ( 1.0f - sLight.cutOff ) );
  }
  else 
  {
    return vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
  }
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////
void main ( void )
{
  // My simple magic :)))) ... ( иначе шейдер считает что этих переменных нет )
  gDirectionalLight.base.color;
  gDirectionalLight.base.ambientIntensity;
  gDirectionalLight.base.diffuseIntensity;

  gPointLights[ 0 ].base.color;
  gPointLights[ 0 ].base.ambientIntensity;
  gPointLights[ 0 ].base.diffuseIntensity;
  // ...magic end

  
  vec3 normal = CalcBumpedNormal ( );
  vec4 totalLight = CalcDirectionalLight ( normal );
  
  for ( int i = 0; i < gNumPointLights; i++ ) 
  {
    totalLight += CalcPointLight ( gPointLights[ i ], normal, lightSpacePos );
  }

  for ( int i = 0 ; i < gNumSpotLights; i++ ) 
  {
    totalLight += CalcSpotLight ( gSpotLights[ i ], normal, lightSpacePos );
  }
  
  vec4 samplerColor = texture2D ( gColorMap, texCoord0.xy );
  fragColor = samplerColor * totalLight;
}
