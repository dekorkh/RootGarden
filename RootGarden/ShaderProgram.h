#pragma once
#include "Shader.h"
#include "TTLMUniform.h"

#include "Eigen/Dense"
#include <map>

using namespace Eigen;
using namespace std;

class ShaderManager;

class ShaderProgram
{
public:
	ShaderProgram(Shader *InVertexShader, Shader *InFragmentShader);
	~ShaderProgram();

	/* Use
	/ Select the shader program.
	*/
	void Use();
	
	/* Free
	/ Release the program and resources.
	*/
	void Free();

	void AddUniform(string const UniformName, UniformType Type);
	void SetUniform(string const UniformName, GLfloat const * Data) const;

	GLint GetVertexAttribLocation(string AttribName);

	GLint ProgAddr;

private:
	Shader *VertexShader;
	Shader *FragmentShader;

	map<string, TTLMUniform*> UniformMap;

	/* Compile
	/ Attach the shaders and compile program.
	*/
	bool Compile();
};