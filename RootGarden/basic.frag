#version 430 core

in vec4 fragColor;
out vec4 fColor;

void
main()
{
	fColor = fragColor;
}