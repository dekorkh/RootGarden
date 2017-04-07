#include "Scene_Fabric.h"
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

Scene_Fabric::Scene_Fabric()
{
	
	int numCubes = rand() % 50;
	for (int i = 0; i < numCubes; i++)
	{
	CompOutlineCube* Cube = new CompOutlineCube();

	AddChild(Cube);
	}
	
	/*
	MatterRectangle* Rectangle = new MatterRectangle();
	Rectangle->SetScale(Vector3f::Constant(0.1f));
	Rectangle->bIsStencil = true;
	AddChild(Rectangle);
	*/

	MatterRectangle* Rectangle = new MatterRectangle();
	Rectangle->SetScale(Vector3f::Constant(0.1f));
	AddChild(Rectangle);


	Animation_FrameTime* pAnim = new Animation_FrameTime(32.0);
	GLint FrameTime_InputIdx = RegisterInput_Float(FrameTime);
	pAnim->LinkOutput(pAnim->OutputIdx_Out, FrameTime_InputIdx, EOperationType::ACCUMULATE);
	InputMap[FrameTime_InputIdx]->InputEffects.bComponentParameters = true;
	AddAnimation(pAnim);
}


Scene_Fabric::~Scene_Fabric()
{
}

void Scene_Fabric::Build()
{

}

void Scene_Fabric::HandleInput(int Key, int x, int y, bool down)
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
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}