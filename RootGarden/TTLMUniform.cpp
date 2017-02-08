#include "ShaderProgram.h"
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
	OutLocation = ShaderProgram::glGetUniformLocation_checked(ProgramAddress, UniformName.c_str());
	if (OutLocation < 0)
	{
		GLint CurrentProgAddr = -1;
		glGetIntegerv(GL_CURRENT_PROGRAM, &CurrentProgAddr);
		cout << "Failed to locate active uniform buffer - " << UniformName << " - progaddr: " << CurrentProgAddr << endl;
		cout << "If the uniform buffer decl. in the shader appears valid, and program is linked succ.";
		cout << "and the active program is the program in question... Check that the shader uses the ";
		cout << "uniform as it may be optimized out.";
		Result = false;
	}
	return Result;
}

bool TTLMUniform::PushUniform(ShaderProgram const * InShaderProgram)
{
	bool Result = true;
	if (bDirty)
	{
		Result = RetrieveUniformLocation(InShaderProgram->ProgAddr, UniformName, UniformLocation);

		if (Result)
		{
			switch (Type)
			{
			case UNIFORM_F1:
				ShaderProgram::glUniform1f_checked(*InShaderProgram, UniformName, UniformLocation, *pData);
				break;
			case UNIFORM_F2:
				ShaderProgram::glUniform2f_checked(*InShaderProgram, UniformName, UniformLocation, pData[0], pData[1]);
				break;
			case UNIFORM_F3:
				ShaderProgram::glUniform3f_checked(*InShaderProgram, UniformName, UniformLocation, pData[0], pData[1], pData[2]);
				break;
			case UNIFORM_M4:
				ShaderProgram::glUniformMatrix4fv_checked(*InShaderProgram, UniformName, UniformLocation, 1, false, pData);
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

void TTLMUniform::SetUniformDirty(bool Value)
{
	bDirty = Value;
}