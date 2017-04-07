#version 430 core

in vec4 fragColor;
in vec2 texCoords;

out vec4 fColor;

uniform sampler2D sampler_Color;

void
main()
{
	fColor = fragColor * texture(sampler_Color, texCoords);
}