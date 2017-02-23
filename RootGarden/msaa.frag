#version 430 core

sample in vec4 fragColor;
out vec4 fColor;

void
main()
{
	fColor = fragColor;
}