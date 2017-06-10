#include "MeshPlot.h"

#define _USE_MATH_DEFINES
#include "math.h"

MeshPlot::MeshPlot() :
	Mesh(NUM_EBUFFERS, 0),
	Color(Vector4f::Constant(1.0f)),
	Center(Vector3f::Constant(0.0f)),
	Width(3.2f),
	Height(2.0f),
	DivSpacing_X(1.0f),
	DivSpacing_Y(1.0f),
	Division_Height(0.1f)
{
	Vector3f NewCenter;
	NewCenter << 0.0f, 0.0f, 0.0f;
	SetCenter(NewCenter);

	Vector4f Color;
	Color << 0.2f, 0.2f, 0.2f, 1.0f;
	SetColor(Color);
	MatterType = GL_LINES;
}


MeshPlot::~MeshPlot()
{
}

void MeshPlot::SetColor(const Vector4f& InColor)
{
	Color = InColor;
}

void MeshPlot::SetCenter(const Vector3f& InCenter)
{
	Center = InCenter;
}

void MeshPlot::GenerateMesh_Colors()
{
	ColorsData.clear();

	MeshPlotGridInfo plotGridInfo{};
	plotGridInfo.Compute(Center, Width, Height, DivSpacing_X, DivSpacing_Y);

	for (GLuint i = 0; i < 4 + plotGridInfo.numAllX * 2 + plotGridInfo.numAllY * 2; i++)
	{
		ColorsData.insert(ColorsData.end(), { Color(0), Color(1), Color(2), Color(3) });
	}
}

void MeshPlot::GenerateMesh_Indices()
{
	IndexData.clear();

	MeshPlotGridInfo plotGridInfo{};
	plotGridInfo.Compute(Center, Width, Height, DivSpacing_X, DivSpacing_Y);

	NumVertices = 4 + plotGridInfo.numAllX * 2 + plotGridInfo.numAllY * 2;
	NumVertIndices = NumVertices;

	// X-Axis
	IndexData.insert(IndexData.end(), 0);
	IndexData.insert(IndexData.end(), 1);

	// Y-Axis
	IndexData.insert(IndexData.end(), 2);
	IndexData.insert(IndexData.end(), 3);

	// X major lines
	for (GLuint divx = 0; divx < plotGridInfo.numAllX; ++divx)
	{
		//4 for both axis and then 2 per mark
		IndexData.insert(IndexData.end(), 4 + divx * 2);		
		IndexData.insert(IndexData.end(), 4 + divx * 2 + 1);
	}

	// Y major lines
	for (GLuint divy = 0; divy < plotGridInfo.numAllY; ++divy)
	{
		//4 for both axis, all of x, and then 2 per mark
		IndexData.insert(IndexData.end(), 4 + plotGridInfo.numAllX * 2 + divy * 2);
		IndexData.insert(IndexData.end(), 4 + plotGridInfo.numAllX * 2 + divy * 2 + 1);
	}
}

void MeshPlot::GenerateMesh_Positions()
{
	PositionsData.clear();

	// X-Axis
	PositionsData.insert(PositionsData.end(), { 
		Width * -0.5f, Center.y(), Center.z(),
		Width *  0.5f, Center.y(), Center.z()
	});

	// Y-Axis
	PositionsData.insert(PositionsData.end(), {
		Center.x(), Height * -0.5f, Center.z(),
		Center.x(), Height *  0.5f, Center.z()
	});

	MeshPlotGridInfo plotGridInfo{};
	plotGridInfo.Compute(Center, Width, Height, DivSpacing_X, DivSpacing_Y);

	// X major lines - Positive
	for (GLuint divx = 0; divx < plotGridInfo.numPosX; ++divx)
	{
		GLfloat x = Center.x() + (divx + 1) * DivSpacing_X;
		PositionsData.insert(PositionsData.end(), {
			x, Center.y() + Division_Height * -0.5f, Center.z(),
			x, Center.y() + Division_Height *  0.5f, Center.z()
		});
	}

	// X major lines - Negative
	for (GLuint divx = 0; divx < plotGridInfo.numNegX; ++divx)
	{
		GLfloat x = Center.x() - (divx + 1) * DivSpacing_X;
		PositionsData.insert(PositionsData.end(), {
			x, Center.y() + Division_Height * -0.5f, Center.z(),
			x, Center.y() + Division_Height *  0.5f, Center.z()
		});
	}

	// Y major lines positive
	for (GLuint divy = 0; divy < plotGridInfo.numPosY; ++divy)
	{
		{
			GLfloat y = Center.y() + (divy + 1) * DivSpacing_Y;
			PositionsData.insert(PositionsData.end(), {
				Center.x() - Division_Height * -0.5f, y, Center.z(),
				Center.x() - Division_Height *  0.5f, y, Center.z()
			});
		}
	}

	// Y major lines negative
	for (GLuint divy = 0; divy < plotGridInfo.numNegY; ++divy)
	{
		{
			GLfloat y = Center.y() - (divy + 1) * DivSpacing_Y;
			PositionsData.insert(PositionsData.end(), {
				Center.x() - Division_Height * -0.5f, y, Center.z(),
				Center.x() - Division_Height *  0.5f, y, Center.z()
			});
		}
	}
}

void MeshPlot::GenerateMesh_TexCoords()
{
	TexCoordsData.clear();
}

void MeshPlot::SetUnitScale(Vector3f const &InUnitScale)
{
	DivSpacing_X *= InUnitScale.x();
	DivSpacing_Y *= InUnitScale.y();
	bDirty_Positions	= true;
	bDirty_Indices		= true;
	bDirty_Colors		= true;
	bDirty_TexCoords	= true;
}