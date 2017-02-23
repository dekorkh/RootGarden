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
	IncrementVelocityUp		<< 0.0f,  0.02f, 0.0f;
	IncrementVelocityDown	<< 0.0f, -0.02f, 0.0f;
	IncrementVelocityLeft	<<  0.02f, 0.0f, 0.0f;
	IncrementVelocityRight	<< -0.02f, 0.0f, 0.0f;

	pRoot = new MatterCircle();
	pRoot->SetScale(Vector3f::Constant(0.03f));
	AddChild(pRoot);

	/*
	int numCubes = rand() % 50;
	for (int i = 0; i < numCubes; i++)
	{
		CompOutlineCube* Cube = new CompOutlineCube();
		
		AddChild(Cube);
	}
	*/
	
	Selector = new CompSelector();
	Selector->Select(*pRoot);
	AddChild(Selector);
	

	/*
	MatterRectangle* Rectangle = new MatterRectangle();
	Rectangle->SetScale(Vector3f::Constant(0.1f));
	Rectangle->bIsStencil = true;
	AddChild(Rectangle);
	*/

	CompWater* pWater = new CompWater(*pRoot);
	pWater->SetMaxDrops(128);
	pWater->SetSpawnRate(100.0f);
	pWater->SetBounds(1.6f, 1.0f);
	AddChild(pWater);	
}


Scene_BeanGarden::~Scene_BeanGarden()
{
}

void Scene_BeanGarden::HandleInput(int Key, int x, int y, bool down)
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
				Selector->SelectUp();
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
				Selector->SelectRight();
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
				Selector->SelectDown();
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
				Selector->SelectLeft();
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
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}