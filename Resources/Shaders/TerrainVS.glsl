#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 TextureUV;
layout(location = 2) in vec3 vertexNormal;


out vec2 TexUV;

uniform mat4 World;
uniform mat4 View;
uniform mat4 Projection;


void main()
{
	gl_Position = Projection * View *  World * vec4(vertexPosition, 1.0);
		
	TexUV = TextureUV;	
}

