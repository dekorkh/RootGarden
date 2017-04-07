#include "TextureInfo.h"
#include "Eigen/Dense"

using namespace Eigen;

TextureInfo::TextureInfo() :
	Levels(1),
	Width(64),
	Height(64),
	Format(GL_RGB8)
{
	Data = new GLfloat[Width * Height * 3]();
	for (int i = 0; i < (Width * Height); i++)
	{
		// RED
		int oddRowOffset = i / Width % 2 ? 0 : 1;
		Data[3 * i] = static_cast<GLfloat>((i + oddRowOffset) % 2);
	}

	for (int i = 0; i < (Width * Height); i++)
	{
		// Green

		Data[3 * i + 1] = static_cast<GLfloat>(i) / (Width * Height - 1);
	}

	for (int i = 0; i < (Width * Height); i++)
	{
		// Blue
		float x = static_cast<float>(i % Width) / Width;
		float y = static_cast<float>(i / Width) / Height;
		Vector2f pos;
		pos << x, y;
		Vector2f target;
		target << 0.5f, 0.5f;
		Data[3 * i + 2] = (target - pos).norm() / 0.5f * 1.0f;
	}
}

TextureInfo::~TextureInfo()
{
	delete[] Data;
}

GLsizeiptr TextureInfo::Size()
{
	GLsizeiptr PixelSize = 3 * sizeof(GLfloat);	// Size of GL_RGB8 pxel
	return PixelSize * Width * Height;
}