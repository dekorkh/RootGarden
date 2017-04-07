#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include "string"


using namespace std;

enum UniformType {
	UNIFORM_I1,
	UNIFORM_F1,
	UNIFORM_F2,
	UNIFORM_F3,
	UNIFORM_M4
};

class TTLMUniform
{
public:
	TTLMUniform(string const UniformName, UniformType const Type);
	~TTLMUniform();

	/* SetUniformValue
	/	Set the value on the object.
	*/
	void SetUniformData(GLfloat const * Data);
	void SetUniformData(GLint const * Data);

	/* SetUniform
	/	Copy the uniform to the gpu.
	*/
	bool PushUniform(class ShaderProgram const * InShaderProgram);
	
	/* SetUniformDirty
	* Will set this uniform's bDirty to Value.
	*/
	void SetUniformDirty(bool Value);

private:
	static bool RetrieveUniformLocation(GLint const ProgramAddress, const string& UniformName, GLint &OutLocation);

	string UniformName;
	GLint UniformLocation;
	UniformType Type;
	GLfloat const * pData_f;
	GLint const * pData_i;
	bool bDirty;
};