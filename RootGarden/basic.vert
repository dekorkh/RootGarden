#version 430 core


uniform mat4 ModelMatrix;
uniform float AspectRatio;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
out vec4 fragColor;

void
main()
{	
	vec3 preAspect = (vPosition * ModelMatrix).xyz * vec3(1.0 / AspectRatio, 1.0, 1.0);
	gl_Position =  vec4(preAspect, 1.0);
	//gl_Position = vPosition;
	
	fragColor = vColor * vColor;
}