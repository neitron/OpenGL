#version 330
 
in vec2 texCoordOut;
uniform sampler2D gShadowMap;
 
out vec4 fragColor;
 
void main()
{
    float depth = texture2D ( gShadowMap, texCoordOut ).x; // delete "2D"
    depth = 1.0 - ( 1.0 - depth ) * 25.0;
    fragColor = vec4( depth );
}