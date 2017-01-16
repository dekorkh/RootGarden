#include "MeshRing.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshRing::MeshRing() : Color(Vector4f::Constant(1.0f)), Thickness(0.02f)
{
	Vector4f Red;
	Red << 1.0f, 0.0f, 0.0f, 1.0f;
	SetColor(Red);
	SetNumSegments(30);
	MatterType = GL_TRIANGLES;
}


MeshRing::~MeshRing()
{
}

void MeshRing::SetNumSegments(GLint InNumSegments)
{
	NumSegments = InNumSegments;
}

void MeshRing::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}


void MeshRing::GenerateMesh_Positions()
{
	PositionsData.clear();

	float InnerRadiusPct = 1.0f - Thickness / 2.0f;

	for (GLint i = 0; i < (NumSegments * 2); i++)
	{
		float pctCircle = static_cast<float>(i) / (NumSegments * 2);
		float radians = pctCircle * 2 * static_cast<float>(M_PI);
		
		float xb = sin(radians);
		float yb = cos(radians);

		float xa = xb * InnerRadiusPct;
		float ya = yb * InnerRadiusPct;

		PositionsData.insert(PositionsData.end(), { xa, ya, 0.0f });
		PositionsData.insert(PositionsData.end(), { xb, yb, 0.0f });
	}
}

void MeshRing::GenerateMesh_Colors()
{
	ColorsData.clear();

	for (GLuint i = 0; i < NumVertices; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshRing::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = NumSegments * 4;
	NumVertIndices = NumSegments * 6;

	for (GLint i = 0; i < NumSegments; i++)
	{
		// first triangle
		int ta0 = i * 4;
		int ta1 = ta0 + 1;
		int ta2 = ta0 + 2;

		// second triangle
		int tb0 = ta2;
		int tb1 = ta1;
		int tb2 = ta2 + 1;

		IndexData.insert(IndexData.end(), ta0);
		IndexData.insert(IndexData.end(), ta1);
		IndexData.insert(IndexData.end(), ta2);
		IndexData.insert(IndexData.end(), tb0);
		IndexData.insert(IndexData.end(), tb1);
		IndexData.insert(IndexData.end(), tb2);
	}
}