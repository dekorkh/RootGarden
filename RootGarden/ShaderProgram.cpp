#include "ShaderProgram.h"
#include "time.h"
#include "ShaderManager.h"

#define MAP_ERROR(x) {x, #x}

GLint ShaderProgram::LastProgAddr = -1;
ShaderProgram * ShaderProgram::LastProgPtr = nullptr;
map<string, TTLMUniform*> ShaderProgram::GlobalUniformMap;
map<GLenum, string> ShaderProgram::GLErrorMap = {
	MAP_ERROR(GL_INVALID_ENUM),
	MAP_ERROR(GL_INVALID_VALUE),
	MAP_ERROR(GL_INVALID_OPERATION)
};


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
	LastProgPtr = this;
	ShaderProgram::glUseProgram_checked(ProgAddr);
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
		it.second->PushUniform(this);
	}
}

void ShaderProgram::PushGlobalUniforms()
{
	for (auto& it : GlobalUniformMap)
	{
		it.second->PushUniform(this);
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
		cout << "Error glBufferSubData(): " << GLErrorMap[err];
		throw err;
	}
}

GLint ShaderProgram::glGetAttribLocation_checked(GLint ProgAddr, string const AttribName)
{
	GLenum err;
	GLint loc = glGetAttribLocation(ProgAddr, AttribName.data());
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGetAttribLocation(): " << GLErrorMap[err];
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
		cout << "Error after glBinddBuffer(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glBufferData_checked(GLenum Target, GLsizeiptr Size, GLvoid *Data, GLenum Usage)
{
	GLenum err;
	glBufferData(Target, Size, Data, Usage);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glBufferData(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glEnableVertexAttribArray_checked(GLuint Loc)
{
	GLenum err;
	glEnableVertexAttribArray(Loc); // Enable fetching from this matrix column
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glEnableVertexAttribArray(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glVertexAttribDivisor_checked(GLuint Loc, GLuint Divisor)
{
	GLenum err;
	glVertexAttribDivisor(Loc, Divisor);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glVertexAttribDivisor(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glVertexAttribPointer_checked(GLuint Index, GLint Size, GLenum Type, GLboolean Normalize, GLsizei Stride, const GLvoid *Offset)
{
	GLenum err;
	glVertexAttribPointer(Index, Size, Type, Normalize, Stride, Offset);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glVertexAttribPointer(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glDrawElementsInstanced_checked(GLenum Mode, GLsizei Count, GLenum Type, GLvoid *Indices, GLsizei PrimCount)
{
	GLenum err;
	glDrawElementsInstanced(Mode, Count, Type, Indices, PrimCount);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glDrawElementsInstanced(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glGenVertexArrays_checked(GLsizei Count, GLuint* Arrays)
{
	GLenum err;
	glGenVertexArrays(Count, Arrays);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGenVertexArrays(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glBindVertexArray_checked(GLuint Array)
{
	GLenum err;
	glBindVertexArray(Array);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glBindVertexArray(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glGenBuffers_checked(GLsizei Count, GLuint *Buffers)
{
	GLenum err;
	glGenBuffers(Count, Buffers);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGenBuffers(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glGetActiveUniform_checked(GLuint ProgramAddress, GLint Loc, GLsizei MaxNameLength, GLsizei *ActualNameLength, GLint *Size, GLenum *Type, GLchar *Name)
{
	GLenum err;
	glGetActiveUniform(ProgramAddress, Loc, MaxNameLength, ActualNameLength, Size, Type, Name);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGetActiveUniform(): " << err << "\n";
		if (err == 1281)
		{
			GLint NumActiveUniforms = 0;
			glGetProgramiv(ProgramAddress, GL_ACTIVE_UNIFORMS, &NumActiveUniforms);
			if (NumActiveUniforms < Loc)
			{
				cout << "Program has " << NumActiveUniforms << " active uniforms, requested index: " << Loc << "\n";
				throw;
			}
		}
		throw;
	}
}

void ShaderProgram::glUniform1f_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLfloat Value)
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
				cout << "\tNo active program object.\n";
			if (InShaderProgram.GetUniformType(UniformName) != EUNIFORM_TYPE::UNIFORM_TYPE_FLOAT)
				cout << "\tUniform declared in shader is not float type.\n";

			GLsizei ActualUniformNameLength = 0;
			GLint UniformSize = 0;
			GLenum UniformType;
			GLchar ActualUniformName[128];
			ShaderProgram::glGetActiveUniform_checked(prog_addr, 
				Loc, 
				UniformName.size(), 
				&ActualUniformNameLength,
				&UniformSize,
				&UniformType,
				ActualUniformName);
			cout << "";
		}
		throw err;
	}
}

void ShaderProgram::glUniform2f_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLfloat Value1, GLfloat Value2)
{
	GLenum err;
	glUniform2f(Loc, Value1, Value2);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniform2f(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glUniform3f_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLfloat Value1, GLfloat Value2, GLfloat Value3)
{
	GLenum err;
	glUniform3f(Loc, Value1, Value2, Value3);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniform3f(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glUniformMatrix4fv_checked(ShaderProgram const &InShaderProgram, string const &UniformName, GLuint Loc, GLsizei Count, GLboolean Transpose, GLfloat const *ValueArray)
{
	GLenum err;
	glUniformMatrix4fv(Loc, Count, Transpose, ValueArray);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUniformMatrix4fv(): " << GLErrorMap[err];
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
		cout << "Error at glGetUniformLocation(): " << GLErrorMap[err];
		throw err;
	}
	return OutLocation;
}

EUNIFORM_TYPE ShaderProgram::GetUniformType(string const UniformName) const
{
	return VertexShader->GetUniformType(UniformName);
}

void ShaderProgram::glUseProgram_checked(GLuint ProgramAddress)
{
	GLenum err;
	glUseProgram(ProgramAddress);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glUseProgram(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glGenQueries_checked(GLsizei n, GLuint *ids)
{
	GLenum err;
	glGenQueries(n, ids);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGenQueries(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glBeginQuery_checked(GLenum target, GLuint id)
{
	GLenum err;
	glBeginQuery(target, id);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glBeginQuery(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glEndQuery_checked(GLenum target)
{
	GLenum err;
	glEndQuery(target);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glEndQuery(): " << GLErrorMap[err];
		throw err;
	}
}

void ShaderProgram::glGetQueryObjectiv_checked(GLenum id, GLenum pname, GLint *params)
{
	GLenum err;
	glGetQueryObjectiv(id, pname, params);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGetQueryObjectiv(): " << GLErrorMap[err] << "\n";
		if (err == GL_INVALID_OPERATION)
		{
			if (glIsQuery_checked(id) == GL_FALSE)
			{
				cout << "\tid is not a valid query object." << "\n";
				throw err;
			}
		}
		throw err;
	}
}

void ShaderProgram::glGetQueryObjectuiv_checked(GLenum id, GLenum pname, GLuint *params)
{
	GLenum err;
	glGetQueryObjectuiv(id, pname, params);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glGetQueryObjectuiv(): " << GLErrorMap[err];
		throw err;
	}
}

GLboolean ShaderProgram::glIsQuery_checked(GLenum id)
{
	GLenum err;
	return glIsQuery(id);
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		cout << "Error at glIsQuery(): " << GLErrorMap[err];
		throw err;
	}
}