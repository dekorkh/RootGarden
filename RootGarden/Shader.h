#pragma once

#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include "GL/freeglut.h"
#include "glext.h"
#include "wglext.h"

#define MAX_LINE_LENGTH 1024
#define MAX_LINE_COUNT 1024

using namespace std;

enum ERROR_TYPE {
	TG_ERROR_NONE,
	TG_ERROR_OPENFILE,
	TG_ERROR_READFILE,
	TG_ERROR_SHADERCOMPILE,
};

enum ESHADER_TYPE {
	SHADER_TYPE_VERTEX,
	SHADER_TYPE_FRAGMENT
};

class Shader
{
public:
	Shader(char const * const InShaderPath, ESHADER_TYPE const InShaderType);
	~Shader();

	/*	CreateAndCompileShader: Creates a shader gpu resource/address, sets the source to its shader address
	/	and compiles it.  Returns true on success, false on failure and sets _lastErr to ERROR_TYPE.
	*/
	bool CreateAndCompileShader();

	/* FreeShader 
	/ Deletes/frees the shader resource on the gpu (do once the program is compiled.)
	*/
	void FreeShader();

	/* AttachShader
	/ Attaches the shader to the program.
	*/
	void AttachShader(GLuint ProgAddr);

private:
	/*	readInTextFile: Attempts to read in a file from pathName, set it to a shader address
	/	and compile it.  Returns number of lines read in on succes or -1 on failure and
	/	sets _lastErr to ERROR_TYPE.
	/	PARAMS:
	/		filePath - The path to the text file.
	/		outData - pointer-to-pointer of characters that will be filled with file data
	/		numLines - The maximum number of lines to read in.
	/		OutResult - Whether the read was successfull as an ERROR_TYPE.
	*/
	static int ReadInTextFile(char const * const filePath, GLchar const ** outData, int const numLines, ERROR_TYPE &OutResult);

	ERROR_TYPE LastError;
	ESHADER_TYPE ShaderType;
	GLuint ShaderAddr;
	char const * const ShaderPath;
	GLuint NumLines;
	GLchar const ** StreamData;
};