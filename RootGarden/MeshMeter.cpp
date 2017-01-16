#include "MeshMeter.h"

#define _USE_MATH_DEFINES
#include "math.h"
#include <iostream>

MeshMeter::MeshMeter() : Color(Vector4f::Constant(1.0f)), Divisions(11)
{
	MajorLines.insert(MajorLines.end(), { 0, 5, 10 });
	Vector4f Col;
	Col << 0.55f, 0.55f, 0.5f, 1.0f;
	SetColor(Col);
	MatterType = GL_TRIANGLES;
}


MeshMeter::~MeshMeter()
{
}

void MeshMeter::SetColor(const Vector4f& InColor)
{
	Color = InColor;
	bDirty_Colors = true;
}

void MeshMeter::GenerateMesh_Colors()
{
	ColorsData.clear();

	for (GLuint i = 0; i < NumVertices; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshMeter::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = 4 * Divisions;
	NumVertIndices = 6 * Divisions;

	for (GLuint i = 0; i < Divisions; i++)
	{
		BuildRectangle_Indices(IndexData, i * 4);
	}
}

void MeshMeter::GenerateMesh_Positions()
{
	PositionsData.clear();
	
	GLfloat DivThickness = 0.03f;

	GLuint NextMajorLineIdx = 0;

	for (GLuint i = 0; i < Divisions; i++)
	{
		GLfloat yCenter = (1.0f - static_cast<float>(i) / (Divisions - 1) * (2.0f - DivThickness)) - DivThickness / 2.0f;
		GLfloat y1 = yCenter - DivThickness / 2.0f;
		GLfloat y2 = yCenter + DivThickness / 2.0f;

		GLfloat x1 = -0.5f;
		GLfloat x2 =  0.5f;

		if (MajorLines.size() > NextMajorLineIdx && i == MajorLines[NextMajorLineIdx])
		{
			x1 *= 1.5f;
			x2 *= 1.5f;
			NextMajorLineIdx++;
		}

		BuildRectangle_Positions(PositionsData, Vector2f(x1, y1), Vector2f(x2, y2));
	}
}