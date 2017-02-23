#include "MeshTrapezoid.h"



MeshTrapezoid::MeshTrapezoid() 
	: 
	BaseWidth(1.0), 
	TipWidth(0.5f),
	Color(Vector4f::Constant(1.0f))
{
	MatterType = GL_TRIANGLES;
}


MeshTrapezoid::~MeshTrapezoid()
{
}

void MeshTrapezoid::GenerateMesh_Positions()
{
	PositionsData.clear();

	PositionsData.resize(4 * 3);

	PositionsData[3 * 0 + 1] = -0.5f * BaseWidth;
	
	PositionsData[3 * 1 + 1] = 0.5f * BaseWidth;

	PositionsData[3 * 2 + 0] = Length;
	PositionsData[3 * 2 + 1] = -0.5f * TipWidth;
	
	PositionsData[3 * 3 + 0] = Length;
	PositionsData[3 * 3 + 1] = 0.5f * TipWidth;
}

void MeshTrapezoid::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLuint i = 0; i < NumVertices; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshTrapezoid::GenerateMesh_Indices()
{
	IndexData.clear();
	NumVertices = 4;
	NumVertIndices = 6;

	IndexData.insert(IndexData.end(), 0);
	IndexData.insert(IndexData.end(), 1);
	IndexData.insert(IndexData.end(), 2);
	IndexData.insert(IndexData.end(), 2);
	IndexData.insert(IndexData.end(), 1);
	IndexData.insert(IndexData.end(), 3);
}

void MeshTrapezoid::SetBaseWidth(float w)
{
	BaseWidth = w;
}

void MeshTrapezoid::SetTipWidth(float w)
{
	TipWidth = w;
}

void MeshTrapezoid::SetLength(float l)
{
	Length = l;
}