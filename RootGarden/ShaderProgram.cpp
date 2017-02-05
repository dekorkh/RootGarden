#include "ShaderProgram.h"
#include "time.h"
#include "ShaderManager.h"

GLint ShaderProgram::LastProgAddr = -1;
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
		int logLength;
		glGetProgramiv(ProgAddr, GL_INFO_LOG_LENGTH, &logLength);
		char *log = new char[logLength]();
		glGetProgramInfoLog(ProgAddr, logLength, NULL, log);
		cout << log;
		delete[] log;
		throw;
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

void ShaderProgram::SetProgramUniformDirty(string const UniformName, bool const Value)
{
	UniformMap.at(UniformName)->SetUniformDirty(true);
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
	GlobalUniformMap.at(UniformName)->SetUniformDirty(true);
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
		it.second->SetUniformDirty(false);
	}
}

void ShaderProgram::glBufferSubData_checked(GLenum target, GLintptr offset, GLsizeiptr dataSize, GLvoid const *data)
{
	GLenum err;
	int bufferSize = 0;
	int mapped;
	glGetBufferParameteriv(target, GL_BUFFER_SIZE, &bufferSize);
	glGetBufferParameteriv(target, GL_BUFFER_MAPPED, &mapped);
	if ((dataSize + offset) > bufferSize)
	{
		cout << "Error 1281 - Invalid Value at glBufferSubData() - range of data copy requested exceed buffer size";
		throw;
	}
	glBufferSubData(target, offset, dataSize, data);
	if (mapped == GL_TRUE)
	{
		cout << "Error 1281 - Invalid Value glBufferSubData() - buffer is mapped.";
		throw;
	}
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error glBufferSubData(): " << err;
		throw err;
	}
}

GLint ShaderProgram::glGetAttribLocation_checked(GLint ProgAddr, string const AttribName)
{
	GLenum err;
	GLint loc = glGetAttribLocation(ProgAddr, AttribName.data());
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGetAttribLocation(): " << err;
		throw err;
	}
	if (loc == -1)
	{
		cout << "glGetAttribLocation failed to locate active attribute: " << AttribName;
		throw - 1;
	}
	return loc;
}

void ShaderProgram::glBindBuffer_checked(GLenum Target, GLint Buffer)
{
	GLenum err;
	glBindBuffer(Target, Buffer);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error after glBinddBuffer(): " << err;
		throw err;
	}
}

void ShaderProgram::glBufferData_checked(GLenum Target, GLsizeiptr Size, GLvoid *Data, GLenum Usage)
{
	GLenum err;
	glBufferData(Target, Size, Data, Usage);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glBufferData(): " << err;
		throw err;
	}
}

void ShaderProgram::glEnableVertexAttribArray_checked(GLuint Loc)
{
	GLenum err;
	glEnableVertexAttribArray(Loc); // Enable fetching from this matrix column
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glEnableVertexAttribArray(): " << err;
		throw err;
	}
}

void ShaderProgram::glVertexAttribDivisor_checked(GLuint Loc, GLuint Divisor)
{
	GLenum err;
	glVertexAttribDivisor(Loc, Divisor);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glVertexAttribDivisor(): " << err;
		throw err;
	}
}

void ShaderProgram::glVertexAttribPointer_checked(GLuint Index, GLint Size, GLenum Type, GLboolean Normalize, GLsizei Stride, const GLvoid *Offset)
{
	GLenum err;
	glVertexAttribPointer(Index, Size, Type, Normalize, Stride, Offset);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glVertexAttribPointer(): " << err;
		throw err;
	}
}

void ShaderProgram::glDrawElementsInstanced_checked(GLenum Mode, GLsizei Count, GLenum Type, GLvoid *Indices, GLsizei PrimCount)
{
	GLenum err;
	glDrawElementsInstanced(Mode, Count, Type, Indices, PrimCount);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glDrawElementsInstanced(): " << err;
		throw err;
	}
}

void ShaderProgram::glGenVertexArrays_checked(GLsizei Count, GLuint* Arrays)
{
	GLenum err;
	glGenVertexArrays(Count, Arrays);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGenVertexArrays(): " << err;
		throw err;
	}
}

void ShaderProgram::glBindVertexArray_checked(GLuint Array)
{
	GLenum err;
	glBindVertexArray(Array);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glBindVertexArray(): " << err;
		throw err;
	}
}

void ShaderProgram::glGenBuffers_checked(GLsizei Count, GLuint *Buffers)
{
	GLenum err;
	glGenBuffers(Count, Buffers);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGenBuffers(): " << err;
		throw err;
	}
}

void ShaderProgram::glUniform1f_checked(GLuint Loc, GLfloat Value)
{
	GLenum err;
	glUniform1f(Loc, Value);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniform1f(): " << err << "\n";
		if (err == 1282) // INVALID OPERATION
		{
			GLint prog_addr;
			glGetIntegerv(GL_CURRENT_PROGRAM, &prog_addr);
			if (prog_addr == 0)
				cout << "\tNo active program object." << err;
		}
		throw err;
	}
}

void ShaderProgram::glUniform2f_checked(GLuint Loc, GLfloat Value1, GLfloat Value2)
{
	GLenum err;
	glUniform2f(Loc, Value1, Value2);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniform2f(): " << err;
		throw err;
	}
}

void ShaderProgram::glUniform3f_checked(GLuint Loc, GLfloat Value1, GLfloat Value2, GLfloat Value3)
{
	GLenum err;
	glUniform3f(Loc, Value1, Value2, Value3);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniform3f(): " << err;
		throw err;
	}
}

void ShaderProgram::glUniformMatrix4fv_checked(GLuint Loc, GLsizei Count, GLboolean Transpose, GLfloat const *ValueArray)
{
	GLenum err;
	glUniformMatrix4fv(Loc, Count, Transpose, ValueArray);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniformMatrix4fv(): " << err;
		throw err;
	}
}

GLint ShaderProgram::glGetUniformLocation_checked(GLuint ProgramAddress, const GLchar *UniformName)
{
	GLenum err;
	GLint OutLocation;
	OutLocation = glGetUniformLocation(ProgramAddress, UniformName);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGetUniformLocation(): " << err;
		throw err;
	}
	return OutLocation;
}