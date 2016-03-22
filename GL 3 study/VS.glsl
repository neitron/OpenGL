#version 330                                    
                       

layout ( location = 0 ) in vec3 position;	         
layout ( location = 1 ) in vec2 texCoord;
layout ( location = 2 ) in vec3 normal;
layout ( location = 3 ) in vec3 tangent;
									         
uniform mat4 gWVP;     
uniform mat4 gLightWVP;                          
uniform mat4 gWorld; // without VIEW and PROJECTION (VP)

out vec4 lightSpacePos;

out vec3 worldPos0; // for vertex data in world coordinate system shadow map  
out vec2 texCoord0;
out vec3 normal0;
out vec3 tangent0;                               

void main ( void )
{
	gl_Position   = gWVP * vec4 ( position, 1.0 ); // WVP vertices for Camera
	lightSpacePos = gLightWVP * vec4 ( position, 1.0 ); // WVP vertices for light

	texCoord0   = texCoord;
	normal0     = ( gWorld * vec4 ( normal,   0.0 ) ).xyz;
  tangent0    = ( gWorld * vec4 ( tangent,  0.0 ) ).xyz;
	worldPos0   = ( gWorld * vec4 ( position, 1.0 ) ).xyz;
}