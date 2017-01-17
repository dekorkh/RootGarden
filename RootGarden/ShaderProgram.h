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

	/* PushUniforms
	/ Push the uniform data to buffers for the mesh and mark clean.
	*/
	void PushUniforms();

	/* PushGlobalUniforms
	/ Push the uniform data to buffers for the frame. 
	/ Call a separate command to mark them clean at end of frame.
	*/
	void static PushGlobalUniforms(GLint ProgAddr);

	/* MarkGlobalUniformsClean
	/ Mark global uniforms as clean.
	*/
	void static MarkGlobalUniformsClean();

	void AddUniform(string const UniformName, UniformType Type);
	void SetUniform(string const UniformName, GLfloat const * pData) const;
	void MarkUniformDirty(string const UniformName, bool const Value);

	static void AddGlobalUniform(string const UniformName, UniformType Type);
	static void SetGlobalUniform(string const UniformName, GLfloat const * pData);
	static void MarkGlobalUniformDirty(string const UniformName, bool const Value);
	
	GLint GetVertexAttribLocation(string AttribName);

	GLint ProgAddr;
	static GLint LastProgAddr;

private:
	Shader *VertexShader;
	Shader *FragmentShader;

	/* A set of uniforms specific to this object. */
	map<string, TTLMUniform*> UniformMap;

	/* A set of uniforms which are global s.a. aspect ratio. */
	static map<string, TTLMUniform*> GlobalUniformMap;

	/* Compile
	/ Attach the shaders and compile program.
	*/
	bool Compile();
};