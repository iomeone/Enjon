#version 330 core
in vec2 TexCoords;

layout (location = 0) out vec4 diffuse;  	// Diffuse

in DATA
{
	vec4 Position;
	vec2 TexCoords;
	vec4 Color;
} fs_in;

uniform sampler2D texture1;

void main()
{             
    diffuse = fs_in.Color * texture2D(texture1, fs_in.TexCoords);
}