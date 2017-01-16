#include "TTLMUniform.h"

TTLMUniform::TTLMUniform(string const UniformName, UniformType const Type) : UniformName(UniformName), Type(Type)
{
}

TTLMUniform::~TTLMUniform()
{
}

bool TTLMUniform::RetrieveUniformLocation(GLint const ProgramAddress, const string& UniformName, GLint &OutLocation)
{
	bool Result = true;
	OutLocation = glGetUniformLocation(ProgramAddress, UniformName.c_str());
	if (OutLocation < 0)
	{
		cout << "Failed to locate active uniform buffer - " << UniformName << endl;
		Result = false;
	}
	return Result;
}

bool TTLMUniform::SetUniform(const GLint ProgramAddress, GLfloat const * Data) const
{
	bool Result = true;
	GLint UniformLocation;
	Result = RetrieveUniformLocation(ProgramAddress, UniformName, UniformLocation);
	if (Result)
	{
		switch (Type)
		{
		case UNIFORM_F1:
			glUniform1f(UniformLocation, *Data);
			break;
		case UNIFORM_F2:
			glUniform2f(UniformLocation, Data[0], Data[1]);
			break;
		case UNIFORM_F3:
			glUniform3f(UniformLocation, Data[0], Data[1], Data[2]);
			break;
		case UNIFORM_M4:
			glUniformMatrix4fv(UniformLocation, 1, false, Data);
			break;
		default:
			Result = false;
			cerr << "Unexpected uniform type - " << Type << endl;
			break;
		}
	}
	return Result;
}