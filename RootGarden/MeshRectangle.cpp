#include "MeshRectangle.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshRectangle::MeshRectangle() : 
	Mesh(NUM_EBUFFERS, 1),
	Color(Vector4f::Constant(1.0f))
{
	Vector4f Red;
	Red << 1.0f, 0.0f, 0.0f, 1.0f;
	SetColor(Red);
	MatterType = GL_TRIANGLES;
}


MeshRectangle::~MeshRectangle()
{
}

void MeshRectangle::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}

void MeshRectangle::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLuint i = 0; i < 4; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshRectangle::GenerateMesh_Indices()
{
	IndexData.clear();
	
	NumVertices = 4;
	NumVertIndices = 6;

	IndexData.insert(IndexData.end(), 0);
	IndexData.insert(IndexData.end(), 1);
	IndexData.insert(IndexData.end(), 2);
	IndexData.insert(IndexData.end(), 2);
	IndexData.insert(IndexData.end(), 3);
	IndexData.insert(IndexData.end(), 0);
}

void MeshRectangle::GenerateMesh_Positions()
{
	PositionsData.clear();

	PositionsData.insert(PositionsData.end(), { -1.0f, 1.0f, 0.0f });
	PositionsData.insert(PositionsData.end(), { 1.0f, 1.0f, 0.0f });
	PositionsData.insert(PositionsData.end(), { 1.0f, -1.0f, 0.0f });
	PositionsData.insert(PositionsData.end(), { -1.0f, -1.0f, 0.0f });
}

void MeshRectangle::GenerateMesh_TexCoords()
{
	TexCoordsData.clear();

	TexCoordsData.insert(TexCoordsData.end(), { 0.0f, 1.0f });
	TexCoordsData.insert(TexCoordsData.end(), { 1.0f, 1.0f });
	TexCoordsData.insert(TexCoordsData.end(), { 1.0f, 0.0f });
	TexCoordsData.insert(TexCoordsData.end(), { 0.0f, 0.0f });
}