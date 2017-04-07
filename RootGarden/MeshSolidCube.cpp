#include "MeshSolidCube.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshSolidCube::MeshSolidCube() : 
	Color(Vector4f::Constant(1.0f))
{
	Vector4f NewColor;
	NewColor << 1.0f, 0.0f, 0.0f, 1.0f;
	SetColor(NewColor);
	MatterType = GL_TRIANGLES;
}


MeshSolidCube::~MeshSolidCube()
{
}

void MeshSolidCube::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}

void MeshSolidCube::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLuint i = 0; i < 8; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshSolidCube::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = 8;
	NumVertIndices = 36;

	IndexData.insert(IndexData.end(), {
		0,1,2,
		2,3,0,

		1,5,6,
		6,2,1,

		5,4,7,
		7,6,5,

		4,0,3,
		3,7,4,

		3,2,6,
		6,7,3,

		0,5,1,
		0,4,5
	});
}

void MeshSolidCube::GenerateMesh_Positions()
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