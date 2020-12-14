#version 330 core


in vec2 TexUV;
out vec4 PixelColor;

uniform sampler2D ModelTexture;
uniform vec4 ModelColor;

void main()
{	
	//PixelColor = texture( ModelTexture, TexUV );	
	//vec4 Color = vec4(1.0, 0.0, 0.0, 1.0);
	PixelColor = texture2D(ModelTexture, TexUV);

	PixelColor = PixelColor + ModelColor;
	//PixelColor = vec4(1.0, 0.0, 0.0, 1.0);
}