#include "ShaderProgram.h"
#include "time.h"
#include "ShaderManager.h"

GLint ShaderProgram::LastProgAddr = 0;
map<string, TTLMUniform*> ShaderProgram::GlobalUniformMap;

ShaderProgram::ShaderProgram(Shader *InVertexShader, Shader *InFragmentShader) :
	VertexShader(InVertexShader),
	FragmentShader(InFragmentShader)
{
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
	LastProgAddr = ProgAddr;
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

void ShaderProgram::SetUniform(string const UniformName, GLfloat const * pData) const
{
	UniformMap.at(UniformName)->SetUniformData(pData);
}

void ShaderProgram::MarkUniformDirty(string const UniformName, bool const Value)
{
	UniformMap.at(UniformName)->bDirty = true;
}

void ShaderProgram::AddGlobalUniform(string const UniformName, UniformType const Type)
{
	TTLMUniform *NewUniform = new TTLMUniform(UniformName, Type);
	GlobalUniformMap.insert(pair<string, TTLMUniform*>(UniformName, NewUniform));
}

void ShaderProgram::SetGlobalUniform(string const UniformName, GLfloat const * pData)
{
	GlobalUniformMap.at(UniformName)->SetUniformData(pData);
}

void ShaderProgram::MarkGlobalUniformDirty(string const UniformName, bool const Value)
{
	GlobalUniformMap.at(UniformName)->bDirty = true;
}

GLint ShaderProgram::GetVertexAttribLocation(string AttribName)
{
	return glGetAttribLocation(ProgAddr, AttribName.data());
}

void ShaderProgram::PushUniforms()
{
	for (auto& it : UniformMap)
	{
		it.second->PushUniform(ProgAddr);
	}
}

void ShaderProgram::PushGlobalUniforms(GLint ProgAddr)
{
	for (auto& it : GlobalUniformMap)
	{
		it.second->PushUniform(ProgAddr);
	}
}

void ShaderProgram::MarkGlobalUniformsClean()
{
	for (auto& it : GlobalUniformMap)
	{
		it.second->bDirty = false;
	}
}