#version 430 core

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform float AspectRatio;

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec3 vNormal;

out vec4 fragColor;

void
main()
{	
	vec3 preAspect = (vPosition * ModelMatrix * ViewMatrix).xyz * vec3(1.0 / AspectRatio, 1.0, 1.0);

	gl_Position =  vec4(preAspect, 1.0) ;
	vec3 DirectionalLight = normalize(vec3(0, 1, 1));
	fragColor = vColor * vColor * (1.0 - dot(vNormal, DirectionalLight)) * 0.00001 + vec4(vNormal.xyz, 1.0);
}