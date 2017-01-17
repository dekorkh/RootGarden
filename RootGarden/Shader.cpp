#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader(char const * const InShaderPath, ESHADER_TYPE const InShaderType) :
	LastError(TG_ERROR_NONE), 
	ShaderPath(InShaderPath),
	ShaderType(InShaderType)
{
	StreamData = new GLchar const *[MAX_LINE_COUNT];
	NumLines = ReadInTextFile(ShaderPath, StreamData, MAX_LINE_COUNT, LastError);
}

Shader::~Shader()
{
	//Release memory
	for (unsigned int line_idx = 0; line_idx < NumLines; line_idx++)
	{
		delete[] StreamData[line_idx]; //allocated to an array
	}
	delete[] StreamData;
}

int Shader::ReadInTextFile(
	char const * const filePath,
	GLchar const ** outData,
	int const numLines,
	ERROR_TYPE &OutResult
)
{
	ifstream inStream = ifstream();
	inStream.open(filePath, ifstream::in);
	if (inStream.fail())
	{
		cout << "Failed to open " << filePath << endl;
		return TG_ERROR_OPENFILE;
	}

	cout << "Reading file: " << filePath << endl;
	int line_idx = 0;
	while (!inStream.eof() && line_idx < numLines)
	{
		GLchar *line_in = new GLchar[MAX_LINE_LENGTH];
		inStream.getline(line_in, MAX_LINE_LENGTH);
		streamsize charsRead = inStream.gcount();
		// Add newline if the line has space
		if (charsRead <= MAX_LINE_LENGTH)
		{
			//reading eof doesn't count towards charsRead unlike end of line
			//so on the last line, charsRead is one char too low, increment it manually
			//to make sure the newline is placed after the last character.
			if (inStream.eof())
				charsRead++;

			line_in[charsRead - 1] = '\n';
			line_in[charsRead] = '\0';
		}

		if (!inStream.fail())
		{
			char smart_endl = (line_in[charsRead - 1] == '\n') ? ' ' : '\n'; //todo: try null instead of space
			cout << line_in << smart_endl;
			outData[line_idx] = line_in;
		}
		else {
			cerr << "Failed to extract character at line: " << line_idx << endl;
			OutResult = TG_ERROR_READFILE;
			return -1;
		}
		line_idx++;
	}
	inStream.close();
	cout << "Read successfull.\n" << endl;
	return line_idx;
}


bool Shader::CreateAndCompileShader()
{
	bool result = true;

	if (NumLines >= 0)
	{
		int type = ShaderType == SHADER_TYPE_VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
		ShaderAddr = glCreateShader(type);

		glShaderSource(ShaderAddr, NumLines, StreamData, NULL);
		glCompileShader(ShaderAddr);

		GLint compileResult = 0;
		glGetShaderiv(ShaderAddr, GL_COMPILE_STATUS, &compileResult);
		if (compileResult != GL_TRUE)
		{
			GLint errLogLength = 0;
			glGetShaderiv(ShaderAddr, GL_INFO_LOG_LENGTH, &errLogLength);
			char* shaderLog = new char[errLogLength];
			glGetShaderInfoLog(ShaderAddr, MAX_LINE_COUNT, NULL, shaderLog);
			cout << "Shader Compile Log Error:\n" << shaderLog << endl;
			delete[] shaderLog;
			LastError = TG_ERROR_SHADERCOMPILE;
			result = false;
		}
	}
	else
	{
		result = false;
	}

	return result;
}

void Shader::FreeShader()
{
	glDeleteShader(ShaderAddr);
}

void Shader::AttachShader(GLuint ProgAddr)
{
	glAttachShader(ProgAddr, ShaderAddr);
}