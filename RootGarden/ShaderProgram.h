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
	static map<GLenum, string> GLErrorMap;
	/* GL methods wrapped with the best error checking, other error checking is the worst
	*/
	static void glBufferSubData_checked(GLenum target, GLintptr offset, GLsizeiptr dataSize, GLvoid const *data);
	static GLint glGetAttribLocation_checked(GLint ProgAddr, string const AttribName);
	static void glBindBuffer_checked(GLenum Target, GLint Buffer);
	static void glBufferData_checked(GLenum Target, GLsizeiptr Size, GLvoid *Data, GLenum Usage);
	static void glEnableVertexAttribArray_checked(GLuint Loc);
	static void glVertexAttribDivisor_checked(GLuint Loc, GLuint Divisor);
	static void glVertexAttribPointer_checked(GLuint Index, GLint Size, GLenum Type, GLboolean Normalize, GLsizei Stride, const GLvoid *Offset);
	static void glDrawElementsInstanced_checked(GLenum Mode, GLsizei Count, GLenum Type, GLvoid *Indices, GLsizei PrimCount);
	static void glGenVertexArrays_checked(GLsizei Count, GLuint* Arrays);
	static void glBindVertexArray_checked(GLuint Array);
	static void glGenBuffers_checked(GLsizei Count, GLuint *Buffers);
	static void glGenTextures_checked(GLsizei Count, GLuint *Textures);
	static void glBindTexture_checked(GLenum Target, GLuint Texture);
	static void glActiveTexture_checked(GLenum Texture);
	static void glTexStorage2D_checked(GLenum Target, GLsizei Levels, GLenum InternalFormat, GLsizei Width, GLsizei Height);
	static void glTexSubImage2D_checked(GLenum Target, GLint Level, GLint XOffset, GLint YOffset, GLsizei Width, GLsizei Height, GLenum Format, GLenum Type, const GLvoid* Data);
	static void glGenSamplers_checked(GLsizei Count, GLuint *Samplers);
	static void glBindSampler_checked(GLuint Unit, GLuint Sampler);
	static void glUniform1i_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLint Loc, GLint Value);
	static void glUniform1f_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLfloat Value);
	static void glUniform2f_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLfloat Value1, GLfloat Value2);
	static void glUniform3f_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLfloat Value1, GLfloat Value2, GLfloat Value3);
	static void glUniformMatrix4fv_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLsizei Count, GLboolean Transpose, GLfloat const *ValueArray);
	static GLint ShaderProgram::glGetUniformLocation_checked(GLuint ProgramAddress, const GLchar *UniformName);
	static void glGetActiveUniform_checked(GLuint ProgramAddress, GLint Loc, GLsizei MaxNameLength, GLsizei *ActualNameLength, GLint *Size, GLenum *Type, GLchar *Name);
	static void glUseProgram_checked(GLuint ProgramAddress);
	static void glGenQueries_checked(GLsizei n, GLuint *ids);
	static void glBeginQuery_checked(GLenum target, GLuint id);
	static void glEndQuery_checked(GLenum target);
	static void glGetQueryObjectiv_checked(GLenum id, GLenum pname, GLint *params);
	static void glGetQueryObjectuiv_checked(GLenum id, GLenum pname, GLuint *params);
	static GLboolean glIsQuery_checked(GLenum id);

	ShaderProgram(Shader *InVertexShader, Shader *InFragmentShader, bool InbSupportsNormals = false);
	~ShaderProgram();

	/* Use
	/ Select the shader program.
	*/
	void Use();
	
	/* Free
	/ Release the program and resources.
	*/
	void Free();

	/*  PushUniforms
	/ Push the uniform data to buffers for the mesh and mark clean.
	*/
	void PushUniforms();

	/* PushGlobalUniforms
	/ Push the uniform data to buffers for the frame. 
	/ Call a separate command to mark them clean at end of frame.
	*/
	void PushGlobalUniforms();

	/* MarkGlobalUniformsClean
	/ Mark global uniforms as clean.
	*/
	void static MarkGlobalUniformsClean();

	void AddUniform(string const UniformName, UniformType Type);
	void SetUniform(string const UniformName, GLfloat const * pData) const;
	void SetUniform(string const UniformName, GLint const * pData) const;
	void SetProgramUniformDirty(string const UniformName, bool const Value);

	EUNIFORM_TYPE GetUniformType(string const UniformName) const;

	static void AddGlobalUniform(string const UniformName, UniformType Type);
	static void SetGlobalUniform(string const UniformName, GLfloat const * pData);
	static void MarkGlobalUniformDirty(string const UniformName, bool const Value);

	GLint ProgAddr;
	static GLint LastProgAddr;
	static ShaderProgram *LastProgPtr;

	bool bSupportsNormals;

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