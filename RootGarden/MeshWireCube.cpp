#include "MeshWireCube.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshWireCube::MeshWireCube() : Color(Vector4f::Constant(1.0f))
{
	Vector4f NewColor;
	NewColor << 0.4f, 0.5f, 0.8f, 1.0f;
	SetColor(NewColor);
	MatterType = GL_LINES;
}


MeshWireCube::~MeshWireCube()
{
}

void MeshWireCube::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}

void MeshWireCube::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLuint i = 0; i < 8; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshWireCube::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = 8;
	NumVertIndices = 24;

	IndexData.insert(IndexData.end(), {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		3,7,
		2,6,
		1,5
	});
}

void MeshWireCube::GenerateMesh_Positions()
{
	PositionsData.clear();

	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, -1.0f });
	PositionsData.insert(PositionsData.end(), {  1.0f, -1.0f, -1.0f });
	PositionsData.insert(PositionsData.end(), {  1.0f,  1.0f, -1.0f });
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, -1.0f });

	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, 1.0f });
	PositionsData.insert(PositionsData.end(), {  1.0f, -1.0f, 1.0f });
	PositionsData.insert(PositionsData.end(), {  1.0f,  1.0f, 1.0f });
	PositionsData.insert(PositionsData.end(), { -1.0f,  1.0f, 1.0f });
}