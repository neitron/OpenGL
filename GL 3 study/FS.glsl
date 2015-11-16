#version 330          
                      
in vec2 TexCoord0;
                                        
out vec4 FragColor;                                 
                                                    
uniform sampler2D gSampler;                         
                                                    
void main()                                         
{   
	// Blur + Black&White
	float step = 0.003;
	vec4 color01 = texture2D(gSampler, TexCoord0);
	vec4 color02 = texture2D(gSampler, vec2(TexCoord0.x + step, TexCoord0.y) );
	vec4 color03 = texture2D(gSampler, vec2(TexCoord0.x - step, TexCoord0.y) );
	vec4 color04 = texture2D(gSampler, vec2(TexCoord0.x, TexCoord0.y + step) );
	vec4 color05 = texture2D(gSampler, vec2(TexCoord0.x, TexCoord0.y - step) );
	vec4 color06 = texture2D(gSampler, vec2(TexCoord0.x + step, TexCoord0.y + step) );
	vec4 color07 = texture2D(gSampler, vec2(TexCoord0.x - step, TexCoord0.y - step) );
	vec4 color08 = texture2D(gSampler, vec2(TexCoord0.x + step, TexCoord0.y - step) );
	vec4 color09 = texture2D(gSampler, vec2(TexCoord0.x - step, TexCoord0.y + step) );      
	                                          
	vec4 colorBlur = (color01 * 2.0 + color02 + color03 + color04 + color05 + color06 + color07 + color08 + color09 ) * 0.1;
	
	float colorWBComponrnt = ( colorBlur.x + colorBlur.y + colorBlur.z ) / 3.0;
	vec4 colorWB = vec4(colorWBComponrnt, colorWBComponrnt, colorWBComponrnt, 1.0);
	 
    FragColor = colorWB;
}