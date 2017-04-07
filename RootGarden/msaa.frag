#version 430 core

sample in vec4 fragColor;
sample in vec2 texCoords;

out vec4 fColor;

void
main()
{
	fColor = fragColor;
}