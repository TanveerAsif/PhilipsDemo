#version 330 core


in vec2 TexUV;
out vec4 PixelColor;

uniform sampler2D TerrainTexture;
uniform vec4 TerrainColor;

void main()
{		
	vec4 Color = vec4(0.2, 0.2, 0.1, 1.0);
	//PixelColor = texture2D(TerrainTexture, TexUV);

	PixelColor = Color;
	//PixelColor = vec4(1.0, 0.0, 0.0, 1.0);
}