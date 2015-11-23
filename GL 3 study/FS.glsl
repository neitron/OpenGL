#version 330          
                      
in vec2 TexCoord0;
                                        
out vec4 FragColor;                                 
                                                    
uniform sampler2D gSampler;                         
uniform float gWave;    	
			       
void main()                                         
{   
	// texture move ()
	vec2 newPos = TexCoord0;
	
	newPos.y = newPos.y + sin ( newPos.x * 3.0 + gWave);

	// Blur + Black&White
	float step = 0.0035;
	vec4 color01 = texture2D(gSampler, newPos);
	vec4 color02 = texture2D(gSampler, vec2(newPos.x + step, newPos.y) );
	vec4 color03 = texture2D(gSampler, vec2(newPos.x - step, newPos.y) );
	vec4 color04 = texture2D(gSampler, vec2(newPos.x, newPos.y + step) );
	vec4 color05 = texture2D(gSampler, vec2(newPos.x, newPos.y - step) );
	vec4 color06 = texture2D(gSampler, vec2(newPos.x + step, newPos.y + step) );
	vec4 color07 = texture2D(gSampler, vec2(newPos.x - step, newPos.y - step) );
	vec4 color08 = texture2D(gSampler, vec2(newPos.x + step, newPos.y - step) );
	vec4 color09 = texture2D(gSampler, vec2(newPos.x - step, newPos.y + step) );      
	                                          
	vec4 colorBlur = (color01 * 2.0 + color02 + color03 + color04 + color05 + color06 + color07 + color08 + color09 ) * 0.1;
	
	float colorWBComponrnt = ( colorBlur.x + colorBlur.y + colorBlur.z ) / 3.0; // Black&Whait
	vec4 colorWB = vec4(colorWBComponrnt, colorWBComponrnt, colorWBComponrnt, 1.0);
	 

    FragColor = color01;
	//FragColor.rgb *= FragColor.a;
	FragColor = vec4(1.0 - FragColor.rgb, 1.0); // inverse
	
}