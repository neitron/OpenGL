#version 330          
                      
in vec2 texCoord0;
in vec3 normal0;          
            
out vec4 fragColor;                                                                 
                                                                                    
struct DirectionalLight                                                             
{                                                                                   
    vec3	color;                                                                     
    float	ambientIntensity;    
	float	diffuseIntensity;
	vec3	direction;                                                     
};                                                                                  
                                                                                    
uniform DirectionalLight	gDirectionalLight;                                         
uniform sampler2D			gSampler;        
			       
void main()                                         
{     
	vec4 ambientColor = vec4(gDirectionalLight.color, 1.0f) *
						gDirectionalLight.ambientIntensity;
	
	float diffuseFactor = 
		dot( normalize( normal0 ), -gDirectionalLight.direction );

	vec4 diffuseColor;
 
	if (diffuseFactor > 0)
	{
		diffuseColor = 
			vec4(gDirectionalLight.color, 1.0f) * 
			gDirectionalLight.diffuseIntensity * 
			diffuseFactor;
	}
	else
	{
	    diffuseColor = vec4(0, 0, 0, 0);
	}

	fragColor =	texture2D(gSampler, texCoord0.xy) * 
				(ambientColor + diffuseColor);

}