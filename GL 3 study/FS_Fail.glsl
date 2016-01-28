#version 330          
                
const int MAX_POINT_LIGHTS = 3;

in vec2 texCoord0;
in vec3 normal0;
in vec3 worldPos0;

out vec4 fragColor;

struct Attenuation                                                             
{
  float constant;
  float linear;
  float expVar;
};

struct BaseLight
{
  float diffuseIntensity;
  float ambientIntensity;
  vec3  color;
};

struct DirectionalLight                                                             
{
  vec3      direction;
};

struct PointLight                                                             
{
  Attenuation   atten;
  vec3		      position;
};


uniform PointLight			  gPointLights [ MAX_POINT_LIGHTS ];
uniform BaseLight         gPointBaseLights [ MAX_POINT_LIGHTS ];

uniform int					      gNumPointLights;

uniform DirectionalLight	gDirectionalLight;
uniform BaseLight         gDirectionalBaseLight;

uniform sampler2D			gSampler;

uniform vec3				gEyeWorldPos;
uniform float				gMatSpecularIntensity;
uniform float				gSpecularPower;

vec4 CalcLightInternal ( BaseLight light, vec3 lightDirection, vec3 normal )
{
  vec4	ambientColor	= vec4 ( light.color, 1.0f ) * light.ambientIntensity;
  float	diffuseFactor	= dot ( normal, -lightDirection );

  vec4 diffuseColor  = vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
  vec4 specularColor = vec4 ( 0.0f, 0.0f, 0.0f, 0.0f );
  
  if ( diffuseFactor > 0.0f ) 
  {
    diffuseColor = vec4 ( light.color, 1.0f ) * light.diffuseIntensity * diffuseFactor;

    vec3  vertexToEye     = normalize ( gEyeWorldPos - worldPos0 );
    vec3  lightReflect    = normalize ( reflect ( lightDirection, normal ) ); // получаем вектор отраженного света
    float specularFactor  = dot ( vertexToEye, lightReflect );

    specularFactor = pow ( specularFactor, gSpecularPower );

    if ( specularFactor > 0.0f ) 
    {
      specularColor = vec4 ( light.color, 1.0f ) * gMatSpecularIntensity * specularFactor;
    }
 }
    return ( ambientColor + diffuseColor + specularColor );
}

vec4 CalcDirectionalLight ( vec3 normal )
{
     return CalcLightInternal ( gDirectionalBaseLight, gDirectionalLight.direction, normal);
}

vec4 CalcPointLight ( int index, vec3 normal )
{
    vec3  lightDirection = worldPos0 - gPointLights[ index ].position;
    float distanceVar = length ( lightDirection );
    lightDirection = normalize ( lightDirection );
 
    vec4  color = CalcLightInternal ( gPointBaseLights[index], lightDirection, normal);
    
    float attenuation =  
                  gPointLights[ index ].atten.constant +
                  gPointLights[ index ].atten.linear * distanceVar +
                  gPointLights[ index ].atten.expVar * distanceVar * distanceVar;
 
    return color / attenuation;
}
			       
void main ( void )
{
  vec3 normal = normalize ( normal0 );
  
  vec4 totalLight = CalcDirectionalLight ( normal );
  
  for ( int i = 0; i < gNumPointLights; i++ ) 
  {
    totalLight += CalcPointLight ( i, normal );
  }
  
  fragColor = texture2D(gSampler, texCoord0) * totalLight;
}