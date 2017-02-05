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
	/*
	MatterRectangle* Rectangle = new MatterRectangle();
	AddChild(Rectangle);
	*/

	CompWater* pWater = new CompWater();
	pWater->SetMaxDrops(1000);
	pWater->SetSpawnRate(30.0f);
	pWater->SetBounds(1.6f, 1.0f);
	AddChild(pWater);
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