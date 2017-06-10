#version 430 core

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform float AspectRatio;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoords;
layout(location = 3) in mat4 vModelMatrix; // mat4 occupies 4 consequtive locations
layout(location = 7) in vec4 vInstanceColor;

out vec4 fragColor;
out vec2 texCoords;

void
main()
{	
	gl_Position = vPosition * vModelMatrix * ViewMatrix * vec4((1.0 / AspectRatio), 1.0, 1.0, 1.0);
	fragColor = vColor * ModelMatrix * vModelMatrix * vPosition * AspectRatio * vInstanceColor * 0.000001 + vInstanceColor;

	texCoords = vTexCoords;
}