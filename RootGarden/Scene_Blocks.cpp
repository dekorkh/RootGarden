#include "Scene_Blocks.h"
#include "Animation_Orbit.h"
#include "Animation_AimOrbit.h"
#include "Animation_Aim.h"
#include "Animation_Blank.h"
#include "Animation_Grow.h"
#include "Animation_Pulse.h"
#include "Animation_FrameTime.h"
#include "QTree.h"

#define _USE_MATH_DEFINES
#include "math.h"

Scene_Blocks::Scene_Blocks() :
	VoxelScale(0.1f),
	VoxelDivisions(1024)
{
	// BACKGROUND PLANE
	Vector4f BackgroundColor;
	//.3,.1,.2 is too red
	// 0.21f, 0.1f, 0.2f, 1.0f is still too saturated / bright
	// 0.15f, 0.1f, 0.14f, 1.0f is too dark
	BackgroundColor << 0.17f, 0.11f, 0.16f, 1.0f;

	Voxels.resize(1024); // Columns
	for (auto &VoxelY : Voxels)
	{
		VoxelY.resize(1024);	// Rows
		for (auto &VoxelZ : VoxelY)
		{
			VoxelZ.resize(1024); // Levels
		}
	}
	
	MatterSolidBeveledCube *Root = new MatterSolidBeveledCube();

	GLint RootGridX = 512;
	GLint RootGridY = 512;
	GLint RootGridZ = 512;

	GLint RootPosX = RootGridX * VoxelScale - VoxelDivisions * VoxelScale * 0.5f;
	GLint RootPosY = RootGridY * VoxelScale - VoxelDivisions * VoxelScale * 0.5f;
	GLint RootPosZ = RootGridZ * VoxelScale - VoxelDivisions * VoxelScale * 0.5f;

	Voxels[RootGridX][RootGridX][RootGridX] = Root;

	Vector3f RootScale;
	RootScale << VoxelScale, VoxelScale, VoxelScale;
	Root->SetScale(RootScale);
	Vector3f RootPosition;
	RootPosition << RootPosX, RootPosY, RootPosZ;
	Root->SetPosition(RootPosition);
	AddChild(Root);

}


Scene_Blocks::~Scene_Blocks()
{
}

void Scene_Blocks::Build()
{
}

void Scene_Blocks::HandleInput(int Key, int x, int y, bool down)
{
	cout << "key: " << Key << "\n";
	switch (Key)
	{
	case 101:	//up
		if (down)
		{
			if (!UKeyDown)
			{
				UKeyDown = true;
			}
		}
		else
		{
			if (UKeyDown)
			{
				UKeyDown = false;
			}
		}
		break;
	case 102:	//right
		if (down)
		{
			if (!RKeyDown)
			{
				RKeyDown = true;
			}
		}
		else
		{
			if (RKeyDown)
			{
				RKeyDown = false;
			}
		}
		break;
	case 103:	//down
		if (down)
		{
			if (!DKeyDown)
			{
				DKeyDown = true;
			}
		}
		else
		{
			if (DKeyDown)
			{
				DKeyDown = false;
			}
		}
		break;
	case 100: //left
		if (down)
		{
			if (!LKeyDown)
			{
				LKeyDown = true;
			}
		}
		else
		{
			if (LKeyDown)
			{
				LKeyDown = false;
			}
		}
		break;
	case 27:
		if (down && !EscDown)
		{
			EndScene();
			EscDown = true;
		}
		else if (EscDown)
		{
			EscDown = false;
		}
		break;
	case 13: // Enter
		break;
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}