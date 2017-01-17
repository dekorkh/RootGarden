#include "TTLMUniform.h"
#include "GameStats.h"
#include "Time.h"

TTLMUniform::TTLMUniform(string const UniformName, UniformType const Type) : 
	UniformName(UniformName), 
	UniformLocation(-1),
	Type(Type),
	bDirty(true)
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

bool TTLMUniform::PushUniform(const GLint ProgramAddress)
{
	bool Result = true;
	if (bDirty)
	{
		if (UniformLocation == -1)
			Result = RetrieveUniformLocation(ProgramAddress, UniformName, UniformLocation);

		if (Result)
		{
			switch (Type)
			{
			case UNIFORM_F1:
				glUniform1f(UniformLocation, *pData);
				break;
			case UNIFORM_F2:
				glUniform2f(UniformLocation, pData[0], pData[1]);
				break;
			case UNIFORM_F3:
				glUniform3f(UniformLocation, pData[0], pData[1], pData[2]);
				break;
			case UNIFORM_M4:
				glUniformMatrix4fv(UniformLocation, 1, false, pData);
				break;
			default:
				Result = false;
				cerr << "Unexpected uniform type - " << Type << endl;
				break;
			}
		}
	}
	return Result;
}

void TTLMUniform::SetUniformData(GLfloat const * Data)
{
	pData = Data;
}