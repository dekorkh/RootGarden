template<T>
TTLMUniform<class T>::TTLMUniform(UniformType Type, const string& UniformName, const T& Data)
{
}

template<class T>
TTLMUniform<T>::~TTLMUniform()
{
}


template<class T>
static bool TTLMUniform<T>::RetrieveUniformLocation(GLint const ProgramAddress, char * const UniformName, GLint &OutLocation)
{
	bool Result = true;
	OutLoc = glGetUniformLocation(ProgramAddress, uniformName);
	if (OutLoc < 0)
	{
		cout << "Failed to locate active uniform buffer - " << uniformName << endl;
		result = false;
	}
	return result;
}

template<class T>
static bool TTLMUniform<T>::SetUniform(GLint const prog_addr, char * const uniformName, const GLfloat* value)
{
	GLint uniLoc;
	bool result = getUniform(prog_addr, uniformName, uniLoc);
	if (result)
	{
		switch(Type)
		{
		UNIFORM_FLOAT_1:
			break;
		UNIFORM_FLOAT_2:
			break;
		UNIFORM_FLOAT_3:
			break;
		UNIFORM_MATRIX_4:
			glUniformMatrix4fv(UniformLocation, 1, false, Data);
			break;
		default:
			result = false;
			cerr << "Unexpected uniform type - " << Type << endl;
			break;
		glUniformMatrix4fv(uniLoc, 1, false, value);
		}
	}
	return result;
}
