#include "ShaderProgram.h"
#include "GameStats.h"
#include "time.h"
#include "ShaderManager.h"

ShaderProgram::ShaderProgram(Shader *InVertexShader, Shader *InFragmentShader) :
	VertexShader(InVertexShader),
	FragmentShader(InFragmentShader)
{
	AddUniform("AspectRatio", UNIFORM_F1);
	AddUniform("ModelMatrix", UNIFORM_M4);
	Compile();
}

ShaderProgram::~ShaderProgram()
{
}

bool ShaderProgram::Compile()
{
	//Setup program (shaders)
	ProgAddr = glCreateProgram();

	VertexShader->AttachShader(ProgAddr);
	FragmentShader->AttachShader(ProgAddr);

	glLinkProgram(ProgAddr);

	GLint LinkResult = 0;
	glGetProgramiv(ProgAddr, GL_LINK_STATUS, &LinkResult);
	if (LinkResult != GL_TRUE)
	{
		cout << "Failed to link program." << endl;
		return false;
	}
	
	return true;
}

void ShaderProgram::Use()
{
	glUseProgram(ProgAddr);
}

void ShaderProgram::Free()
{
	glDeleteProgram(ProgAddr);
}

void ShaderProgram::AddUniform(string const UniformName, UniformType const Type)
{
	TTLMUniform *NewUniform = new TTLMUniform(UniformName, Type);
	UniformMap.insert(pair<string, TTLMUniform*>(UniformName, NewUniform));
}

void ShaderProgram::SetUniform(string const UniformName, GLfloat const * Data) const
{
	clock_t Clock_SetUniforms = clock();
	UniformMap.at(UniformName)->SetUniform(ProgAddr, Data);
	GameStats::GetGameStats()->pThisFrame->Ms_SetUniforms->Increment(TICKS_TO_MS(clock() - Clock_SetUniforms));
}

GLint ShaderProgram::GetVertexAttribLocation(string AttribName)
{
	return glGetAttribLocation(ProgAddr, AttribName.data());
}