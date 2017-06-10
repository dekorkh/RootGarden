#include "MeshTaperedCylinder.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshTaperedCylinder::MeshTaperedCylinder()
	:
	BaseWidth(1.0),
	TipWidth(0.5f),
	Color(Vector4f::Constant(1.0f)),
	NumSides(8),
	NumSegments(2)
{
	MatterType = GL_TRIANGLES;
}


MeshTaperedCylinder::~MeshTaperedCylinder()
{
}

void MeshTaperedCylinder::GenerateMesh_Positions()
{
	PositionsData.clear();

	PositionsData.resize(NumSides * NumSegments * 3);

	for (GLint i_seg = 0; i_seg < NumSegments; ++i_seg)
	{
		for (GLint i_side = 0; i_side < NumSides; ++i_side)
		{
			float theta = static_cast<float>(i_side) / NumSides * M_PI * 2;
			float dist = static_cast<float>(i_seg) / (NumSegments - 1);
			float radius = (dist * TipWidth + (1.0f - dist) * BaseWidth);

			float v0_x = dist * Length;
			float v0_y = sinf(theta) * radius;
			float v0_z = cosf(theta) * radius;
			
			int vertIdx = (i_side + i_seg * NumSides);

			PositionsData[3 * vertIdx + 0] = v0_x;
			PositionsData[3 * vertIdx + 1] = v0_y;
			PositionsData[3 * vertIdx + 2] = v0_z;
		}
	}
}

void MeshTaperedCylinder::GenerateMesh_Colors()
{
	ColorsData.clear();
	for (GLint i_seg = 0; i_seg < NumSegments; ++i_seg)
	{
		for (GLint i_side = 0; i_side < NumSides; ++i_side)
		{
			ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
		}
	}
}

void MeshTaperedCylinder::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = NumSides * NumSegments;
	NumVertIndices = NumSides * 6 * NumSegments;

	for (GLuint i_seg = 0; i_seg < NumSegments; ++i_seg)
	{
		if (i_seg == NumSegments - 1)
		{ 
			continue;
		}

		for (GLuint i_side = 0; i_side < NumSides; ++i_side)
		{
			GLuint t0v0, t0v1, t0v2, t1v0, t1v1, t1v2;
			if (i_side != NumSides - 1)
			{
				t0v0 = i_side + i_seg * NumSides;
				t0v1 = t0v0 + 1;
				t0v2 = i_side + (i_seg + 1) * NumSides;
				
				t1v0 = t0v2;
				t1v1 = t0v1;
				t1v2 = t0v2 + 1;
			}
			else
			{
				t0v0 = i_side + i_seg * NumSides;
				t0v1 = i_seg * NumSides;
				t0v2 = i_side + (i_seg + 1) * NumSides;

				t1v0 = t0v2;
				t1v1 = t0v1;
				t1v2 = (i_seg + 1) * NumSides;
			}
			IndexData.insert(IndexData.end(), { t0v0 , t0v1, t0v2, t1v0, t1v1, t1v2 });
		}
	}
}

void MeshTaperedCylinder::SetBaseWidth(float w)
{
	BaseWidth = w;
}

void MeshTaperedCylinder::SetTipWidth(float w)
{
	TipWidth = w;
}

void MeshTaperedCylinder::SetLength(float l)
{
	Length = l;
}