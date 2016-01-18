#version 330                                    
                       
// data from openGL_VBO
// each of atributes has atribute location ( fixed as here). 
// If we use automatic atrib_location, we must request index_location from shader with "glGetAttribLocation"                       

layout (location = 0) in vec3 position;	         
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
									  
									            
uniform mat4 gWVP;                               
uniform mat4 gWorld;

out vec2 texCoord0;
out vec3 normal0;
out vec3 worldPos0;
                               
void main ( void )
{
	gl_Position = gWVP * vec4(position, 1.0);
	texCoord0 = texCoord;

	normal0 = ( gWorld * vec4( normal, 0.0 ) ).xyz;

	worldPos0   = ( gWorld * vec4( position, 1.0 ) ).xyz;
}