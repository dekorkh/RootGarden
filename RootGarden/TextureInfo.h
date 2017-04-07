#pragma once

#include "GL/glew.h"

class TextureInfo
{
public:
	TextureInfo();
	virtual ~TextureInfo();

	GLsizeiptr Size();
	
	GLsizei Levels;
	GLsizei Width;
	GLsizei Height;
	GLenum Format;
	GLfloat* Data;
};
