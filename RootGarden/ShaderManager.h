#pragma once

#include "ShaderProgram.h"

#include <map>

using namespace std;

class ShaderManager
{
public:
	ShaderManager() {};
	~ShaderManager() {};

	static ShaderManager* GetShaderManager();

	void AddShader(string InShaderName, char const * const InShaderPath, ESHADER_TYPE InShaderType);
	
	void AddShaderProgram(string InShaderProgramName, string VertexShaderName, string FragmentShaderName, bool InbSupportsNormals = false);

	ShaderProgram* const GetShaderProgramByName(string const InShaderProgramName) const;

private:
	static ShaderManager* pShaderManager;

	map<string, Shader*> ShaderMap;
	map<string, ShaderProgram*> ShaderProgramMap;
};