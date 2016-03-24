#version 330 

uniform sampler2D gColorMap;
 
in vec2 texCoord;
out vec4 FragColor;
 
void main()
{
    FragColor = texture2D ( gColorMap, texCoord);
 
    if ( FragColor.r == 0 && FragColor.g == 0 && FragColor.b == 0 ) 
    {
        discard;
    }
}