#include "ShaderManager.h"

ShaderManager *ShaderManager::pShaderManager = nullptr;

ShaderManager* ShaderManager::GetShaderManager()
{
	if (pShaderManager == nullptr)
		pShaderManager = new ShaderManager();
	return pShaderManager;
}

void ShaderManager::AddShader(string InShaderName, char const * const InShaderPath, ESHADER_TYPE InShaderType)
{
	Shader* NewShader = new Shader(InShaderPath, InShaderType);
	NewShader->CreateAndCompileShader();
	ShaderMap.insert(pair<string, Shader*>(InShaderName, NewShader));
}

void ShaderManager::AddShaderProgram(string InShaderProgramName, string VertexShaderName, string FragmentShaderName)
{
	Shader *VertexShader = ShaderMap.at(VertexShaderName);
	Shader *FragmentShader = ShaderMap.at(FragmentShaderName);
	ShaderProgram* NewShaderProgram = new ShaderProgram(VertexShader, FragmentShader);
	ShaderProgramMap.insert(pair<string, ShaderProgram*>(InShaderProgramName, NewShaderProgram));
}

ShaderProgram* const ShaderManager::GetShaderProgramByName(string const InShaderProgramName) const
{
	return ShaderProgramMap.at(InShaderProgramName);
}