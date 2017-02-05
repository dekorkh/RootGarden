#include "MeshRectangleGradient.h"

#define _USE_MATH_DEFINES
#include "math.h"

#include "GameStats.h"
#include "time.h"

MeshRectangleGradient::MeshRectangleGradient() : NumSides(32)
{
	SetNumberOfLayers(6);

	Vector4f ColorA, ColorB, ColorC, ColorD;
	ColorA << 1.0f, 0.0f, 0.0f, 1.0f;
	ColorB << 0.0f, 1.0f, 0.0f, 1.0f;
	ColorC << 0.0f, 0.0f, 1.0f, 1.0f;
	ColorD << 1.0f, 1.0f, 0.0f, 1.0f;
	
	SetLayerColor(0, ColorA);
	SetLayerColor(1, ColorB);
	SetLayerColor(2, ColorC);
	SetLayerColor(3, ColorD);
	SetLayerColor(4, ColorA);
	SetLayerColor(5, ColorB);
	
	MatterType = GL_TRIANGLES;
}


MeshRectangleGradient::~MeshRectangleGradient()
{
}

void MeshRectangleGradient::SetLayerColor(const int Index, const Vector4f& Color)
{
	LayerColors[Index] = Color;
}

void MeshRectangleGradient::SetLayerPosition(const int Index, const float Position)
{
	LayerPositions[Index] = Position;
}

void MeshRectangleGradient::SetNumberOfLayers(int InNumLayers)
{
	LayerColors.clear();
	LayerPositions.clear();

	NumLayers = InNumLayers;
	Vector4f DefaultColor;
	DefaultColor << 1.0f, 1.0f, 1.0f, 1.0f;

	for (GLuint idxLayer = 0; idxLayer < NumLayers; idxLayer++)
	{
		LayerColors.insert(LayerColors.end(), DefaultColor);
	}

	for (GLuint idxLayer = 0; idxLayer < NumLayers; idxLayer++)
	{
		LayerPositions.insert(LayerPositions.end(), static_cast<float>(idxLayer) / (NumLayers - 1));
	}
}

void MeshRectangleGradient::GenerateMesh_Positions()
{
	PositionsData.clear();
	PositionsData.reserve(NumVertices * 3);

	// How much to multiply the radius such that the four vertices of the circle make a square.
	float RadiusMultiplier = 1.0f / cos(static_cast<float>(M_PI) / 4) * 0.5f;

	// Make center vert.
	PositionsData.insert(PositionsData.end(), { 0.0f, 0.0f, 0.0f });

	// rings - Positions
	for (GLuint idxLayer = 1; idxLayer < LayerPositions.size(); idxLayer++)
	{
		for (GLuint idxSide = 0; idxSide < NumSides; idxSide++)
		{
			float pctCircle = static_cast<float>(idxSide) / NumSides;
			float radians = pctCircle * 2 * static_cast<float>(M_PI) + static_cast<float>(M_PI) / 4;
			float x = sin(radians) * RadiusMultiplier * LayerPositions[idxLayer];
			float y = cos(radians) * RadiusMultiplier * LayerPositions[idxLayer];
			
			PositionsData.insert(PositionsData.end(), { x, y, 0.0f });
		}
	}
}

void MeshRectangleGradient::GenerateMesh_Colors()
{
	ColorsData.clear();
	ColorsData.reserve(NumVertices * 4);

	ColorsData.insert(ColorsData.end(), { LayerColors[0](0), LayerColors[0](1), LayerColors[0](2), LayerColors[0](3) });

	// rings - Colors
	for (GLuint idxLayer = 1; idxLayer < LayerColors.size(); idxLayer++)
	{
		for (GLuint idxSide = 0; idxSide < NumSides; idxSide++)
		{
			ColorsData.insert(ColorsData.end(), { LayerColors[idxLayer](0), LayerColors[idxLayer](1), LayerColors[idxLayer](2), LayerColors[idxLayer](3) });
		}
	}
}

void MeshRectangleGradient::GenerateMesh_Indices()
{
	IndexData.clear();

	NumVertices = NumSides * NumLayers + 1;
	// 3 indices per side inner on inner ring and 6 indices on outer ring.
	NumVertIndices = NumSides * 3 + (NumLayers - 2) * 6 * NumSides;

	//  first ring - Indices
	for (GLuint i = 0; i < NumSides; i++)
	{
		IndexData.insert(IndexData.end(), 0);
		IndexData.insert(IndexData.end(), 1 + i);
		if (i != NumSides - 1)
			IndexData.insert(IndexData.end(), 2 + i);
		else
			IndexData.insert(IndexData.end(), 1); //last triangle closes on first virt of the circle
	}

	// other rings - Indices
	for (GLuint idxLayer = 2; idxLayer < NumLayers; idxLayer++)
	{
		for (GLuint idxSide = 0; idxSide < NumSides; idxSide++)
		{
			GLint idx0, idx1, idx2, idx3 = 0;

			if (idxSide != NumSides - 1) // not last vert in a circle
			{
				idx0 = NumSides * (idxLayer - 1) + idxSide + 1;
				idx1 = idx0 + 1;
				idx2 = NumSides * (idxLayer - 2) + idxSide + 1;
				idx3 = idx2 + 1;
			}
			else // last vert in circle
			{
				idx0 = NumSides * (idxLayer - 1) + idxSide + 1;
				idx1 = NumSides * (idxLayer - 1) + 1;
				idx2 = NumSides * (idxLayer - 2) + idxSide + 1;
				idx3 = NumSides * (idxLayer - 2) + 1;
			}

			// first triangle
			IndexData.insert(IndexData.end(), idx0);
			IndexData.insert(IndexData.end(), idx1);
			IndexData.insert(IndexData.end(), idx2);

			// second triangle
			IndexData.insert(IndexData.end(), idx2);
			IndexData.insert(IndexData.end(), idx1);
			IndexData.insert(IndexData.end(), idx3);
		}
	}
}

void MeshRectangleGradient::SetNumberOfSides(int InNumSides)
{
	NumSides = InNumSides;
}