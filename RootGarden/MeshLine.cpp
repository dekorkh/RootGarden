#include "MeshLine.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshLine::MeshLine() :
	Mesh(NUM_EBUFFERS, 0),
	Color(Vector4f::Constant(1.0f))
{
	Vector4f Color;
	Color << 1.0f, 0.5f, 0.0f, 1.0f;
	SetColor(Color);
	MatterType = GL_LINE_STRIP;
}


MeshLine::~MeshLine()
{
}

void MeshLine::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}

void MeshLine::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLuint i = 0; i < PositionsData.size() / 3; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshLine::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = static_cast<GLint>(PositionsData.size() / 3);
	NumVertIndices = static_cast<GLint>(PositionsData.size() / 3);

	// rest
	for (GLuint i = 0; i < PositionsData.size() / 3; ++i)
	{
		IndexData.insert(IndexData.end(), i);
	}
}

void MeshLine::GenerateMesh_Positions()
{
}

void MeshLine::GenerateMesh_TexCoords()
{
	TexCoordsData.clear();
}

void MeshLine::SetPositions(vector<GLfloat> &InPositions)
{
	bDirty_Positions = true;
	bDirty_Colors = true;
	bDirty_Indices = true;

	PositionsData.clear();
	PositionsData.insert(PositionsData.end(), InPositions.begin(), InPositions.end());
}
