#include "MatterTileboard.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "Animation_Pulse.h"
#include "Animation_Blank.h"

MatterTileboard::MatterTileboard() :
	SceneComponent("MatterTileboard", Counter<MatterTileboard>::Count())
{
	Name = "MatterTileboard";

	float TileWidth = 0.3f;
	int NumTilesX = static_cast<int>(floor(2.0f / TileWidth));
	int NumTilesY = static_cast<int>(floor(2.0f / TileWidth));

	for (int ix = 0; ix < NumTilesX; ix++)
	{
		for (int iy = 0; iy < NumTilesY; iy++)
		{
			float PosX = TileWidth * ix + TileWidth * 0.5f - 1.0f;
			float PosY = TileWidth * iy + TileWidth * 0.5f - 1.0f;
			
			MatterRectangleGradient *RectangleGradient = MakeTile();

			Vector3f TilePos;
			TilePos << PosX, PosY, 0.0f;
			RectangleGradient->SetPosition(TilePos);

			Vector3f TileScale;
			TileScale << TileWidth, TileWidth, 1.0f;
			RectangleGradient->SetScale(TileScale);

			AddChild(RectangleGradient);
		}
	}
}

MatterTileboard::~MatterTileboard()
{
}

MatterRectangleGradient* MatterTileboard::MakeTile()
{
	MatterRectangleGradient *RectangleGradient = new MatterRectangleGradient();
	RectangleGradient->Pulse();
	return RectangleGradient;
}