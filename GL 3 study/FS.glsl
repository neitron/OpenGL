#version 330          
                
const int MAX_POINT_LIGHTS  = 3;
const int MAX_SPOT_LIGHTS   = 2;

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

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
  vec3		      position;
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

uniform sampler2D			gSampler;

uniform vec3				gEyeWorldPos;
uniform float				gMatSpecularIntensity;
uniform float				gSpecularPower;



////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcLightInternal ( BaseLight light, vec3 lightDirection, vec3 normal )
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

  return ( ambientColor + diffuseColor + specularColor );
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcDirectionalLight ( vec3 normal )
{
     return CalcLightInternal ( gDirectionalLight.base, gDirectionalLight.direction, normal);
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcPointLight ( PointLight pointLight, vec3 normal )
{
     vec3 lightDirection = worldPos0 - pointLight.position;
     float distanceVar = length ( lightDirection );
     lightDirection = normalize ( lightDirection );
 
     vec4 color = CalcLightInternal ( pointLight.base, lightDirection, normal);
     
     float attenuation =  pointLight.atten.constant +
                          pointLight.atten.linear * distanceVar +
                          pointLight.atten.expVar * distanceVar * distanceVar;
 
     return color / attenuation;
}



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
vec4 CalcSpotLight ( SpotLight sLight, vec3 normal )
{
      vec3 lightToPixel = normalize ( worldPos0 - sLight.base.position);
      float spotFactor = dot ( lightToPixel, sLight.direction );
 
      if ( spotFactor > sLight.cutOff ) 
      {
        vec4 color = CalcPointLight ( sLight.base, normal);
        
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
  vec3 normal = normalize ( normal0 );
  
  gDirectionalLight.base.color;
  gDirectionalLight.base.ambientIntensity;
  gDirectionalLight.base.diffuseIntensity;

  gPointLights[ 0 ].base.color;
  gPointLights[ 0 ].base.ambientIntensity;
  gPointLights[ 0 ].base.diffuseIntensity;

  vec4 totalLight = CalcDirectionalLight ( normal );
  
  for ( int i = 0; i < gNumPointLights; i++ ) 
  {
    totalLight += CalcPointLight ( gPointLights[ i ], normal );
  }

  for ( int i = 0 ; i < gNumSpotLights; i++ ) 
  {
    totalLight += CalcSpotLight ( gSpotLights[ i ], normal );
  }
  
  fragColor = texture2D ( gSampler, texCoord0 ) * totalLight;
}
