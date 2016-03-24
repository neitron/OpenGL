#version 330   
                                                                       
layout ( points ) in;                                                     
layout ( triangle_strip ) out;                                            
layout ( max_vertices = 4 ) out;                                          
                                                                       
uniform mat4 gVP;                                                      
uniform vec3 gCameraPos;                                               
                                                                       
out vec2 texCoord;                                                     
                                                                       
void main()                                                            
{                                                                      
    vec3 pos = gl_in[0].gl_Position.xyz;                               
    vec3 toCamera = normalize ( gCameraPos - pos );                       
    vec3 up = vec3 ( 0.0, 1.0, 0.0 );                                     
    vec3 right = cross ( toCamera, up );                                  
                                                                       
    pos -= ( right * 0.5 );                                              
    gl_Position = gVP * vec4 ( pos, 1.0 );                                
    texCoord = vec2 ( 0.0, 0.0 );                                         
    EmitVertex ( );                                                      
                                                                       
    pos.y += 1.0;                                                      
    gl_Position = gVP * vec4 ( pos, 1.0 );                                
    texCoord = vec2 ( 0.0, 1.0 );                                         
    EmitVertex ( );                                                      
                                                                       
    pos.y -= 1.0;                                                      
    pos += right;                                                      
    gl_Position = gVP * vec4 ( pos, 1.0 );                                
    texCoord = vec2 ( 1.0, 0.0 );                                         
    EmitVertex ( );                                                      
                                                                       
    pos.y += 1.0;                                                      
    gl_Position = gVP * vec4 ( pos, 1.0 );                                
    texCoord = vec2 ( 1.0, 1.0 );                                         
    EmitVertex ( );                                                      
                                                                       
    EndPrimitive();                                                    
}