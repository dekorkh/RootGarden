#include "Scene_BeanGarden.h"
#include "Animation_Orbit.h"
#include "Animation_AimOrbit.h"
#include "Animation_Aim.h"
#include "Animation_Blank.h"
#include "Animation_Grow.h"
#include "Animation_Pulse.h"

#define _USE_MATH_DEFINES
#include "math.h"

Scene_BeanGarden::Scene_BeanGarden()
{
	
	int numCubes = rand() % 50;
	for (int i = 0; i < numCubes; i++)
	{
		CompOutlineCube* Cube = new CompOutlineCube();
		AddChild(Cube);
	}
	
	MatterRectangle* Rectangle = new MatterRectangle();
	Rectangle->SetScale(Vector3f::Constant(0.1f));
	Rectangle->bIsStencil = true;
	AddChild(Rectangle);
	
	/*
	CompWater* pWater = new CompWater();
	pWater->SetMaxDrops(1000);
	pWater->SetSpawnRate(30.0f);
	pWater->SetBounds(1.6f, 1.0f);
	AddChild(pWater);
	*/
	
}


Scene_BeanGarden::~Scene_BeanGarden()
{
}

void Scene_BeanGarden::HandleInput(int Key, int x, int y, bool down)
{
	switch (Key)
	{
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
	case 27: //left
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
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}