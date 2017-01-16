#include "MeshCircle.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshCircle::MeshCircle() : ColorOuter(Vector4f::Constant(1.0f)), ColorInner(Vector4f::Constant(1.0f))
{
	Vector4f Red;
	Red << 1.0f, 0.0f, 0.0f, 0.0f;
	Vector4f Yellow;
	Yellow << 1.0f, 1.0f, 0.0f, 1.0f;
	SetColors(Yellow, Red);
	SetNumSides(128);
	MatterType = GL_TRIANGLES;
}


MeshCircle::~MeshCircle()
{
}

void MeshCircle::SetNumSides(GLint InNumSides)
{
	NumSides = InNumSides;
	bDirty_Indices = true;
	bDirty_Positions = true;
	bDirty_Colors = true;
}

void MeshCircle::SetColors(const Vector4f& InColorInner, const Vector4f& InColorOuter)
{
	ColorOuter = InColorOuter;
	ColorInner = InColorInner;
	bDirty_Colors = true;
}


void MeshCircle::GenerateMesh_Indices()
{
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

void MeshCircle::GenerateMesh_Colors()
{
	Colors.clear();
	
	Colors.insert(Colors.end(), ColorInner);
	
	for (GLint i = 0; i < NumSides; i++)
	{
		Colors.insert(Colors.end(), ColorOuter);
	}
}

void MeshCircle::GenerateMesh_Positions()
{
	Positions.clear();

	Vector3f CenterPosition = Vector3f::Constant(0.0f);
	Positions.insert(Positions.end(), CenterPosition);

	for (GLint i = 0; i < NumSides; i++)
	{
		float pctCircle = static_cast<float>(i) / NumSides;
		float radians = pctCircle * 2 * static_cast<float>(M_PI);
		float x = sin(radians);
		float y = cos(radians);
		Vector3f Position;
		Position << x, y, 0.0f;
		Positions.insert(Positions.end(), Position);
	}
}