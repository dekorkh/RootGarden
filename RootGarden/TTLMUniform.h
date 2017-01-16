#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>
#include "string"


using namespace std;

enum UniformType {
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

	bool SetUniform(const GLint ProgramAddress, GLfloat const * Data) const;
	
private:
	static bool RetrieveUniformLocation(GLint const ProgramAddress, const string& UniformName, GLint &OutLocation);

	string UniformName;
	UniformType Type;
};