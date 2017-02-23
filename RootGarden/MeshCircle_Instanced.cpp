#include "MeshCircle_Instanced.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshCircle_Instanced::MeshCircle_Instanced(Matter const &InMatter) : 
	ColorOuter(Vector4f::Constant(1.0f)), 
	ColorInner(Vector4f::Constant(1.0f)), 
	Mesh_Instanced(InMatter)
{

	SetNumSides(5);
	MatterType = GL_TRIANGLES;

	//bUseDebugData = true;
	//bUseDebugModelMatrix = true;
	//bUseDebugColors = true;
}


MeshCircle_Instanced::~MeshCircle_Instanced()
{
}

void MeshCircle_Instanced::SetNumSides(GLint InNumSides)
{
	NumSides = InNumSides;
	bDirty_Indices = true;
	bDirty_Positions = true;
	bDirty_Colors = true;
}

void MeshCircle_Instanced::SetColors(const Vector4f& InColorInner, const Vector4f& InColorOuter)
{
	ColorInner = InColorInner;
	ColorOuter = InColorOuter;
	bDirty_Colors = true;
}


void MeshCircle_Instanced::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = NumSides + 1;
	NumVertIndices = NumSides * 3;

	for (GLint i = 0; i < NumSides; i++)
	{
		IndexData.insert(IndexData.end(), 0);
		IndexData.insert(IndexData.end(), 1 + i);
		if (i != NumSides - 1)
			IndexData.insert(IndexData.end(), 2 + i);
		else
			IndexData.insert(IndexData.end(), 1); //last triangle closes on first virt of the circle
	}
}

void MeshCircle_Instanced::GenerateMesh_Colors()
{
	ColorsData.clear();

	ColorsData.insert(ColorsData.end(), { ColorInner(0), ColorInner(1), ColorInner(2), ColorInner(3) });

	for (GLint i = 0; i < NumSides; i++)
	{
		ColorsData.insert(ColorsData.end(), { ColorOuter(0), ColorOuter(1), ColorOuter(2), ColorOuter(3) });
	}
}

void MeshCircle_Instanced::GenerateMesh_Positions()
{
	PositionsData.clear();

	// Center position
	PositionsData.insert(PositionsData.end(), {0.0f, 0.0f, 0.0f});

	for (GLint i = 0; i < NumSides; i++)
	{
		float pctCircle = static_cast<float>(i) / NumSides;
		float radians = pctCircle * 2 * static_cast<float>(M_PI);
		float x = sin(radians);
		float y = cos(radians);
		PositionsData.insert(PositionsData.end(), {x, y, 0.0f});
	}
}