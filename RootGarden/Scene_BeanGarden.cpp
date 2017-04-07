#include "Scene_BeanGarden.h"
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

	pWater = new CompWater(*pRoot);
	pWater->SetMaxDrops(40);
	pWater->SetSpawnRate(500.0f);
	pWater->SetBounds(1.6f, 1.0f);
	AddChild(pWater);

	Animation_FrameTime* pAnim = new Animation_FrameTime(32.0);
	GLint FrameTime_InputIdx = RegisterInput_Float(FrameTime);
	pAnim->LinkOutput(pAnim->OutputIdx_Out, FrameTime_InputIdx, EOperationType::ACCUMULATE);
	InputMap[FrameTime_InputIdx]->InputEffects.bComponentParameters = true;
	AddAnimation(pAnim);
}


Scene_BeanGarden::~Scene_BeanGarden()
{
}

void Scene_BeanGarden::Build()
{
	vector<Matter*> FoundMatter;
	
	if (!pWater->Tree->isLeaf)
	{
		pWater->Tree->SearchRegion(Selector->Input_Position, 0.3f, FoundMatter);
	}

	for (auto &Matter : pWater->Children)
	{
		MatterDrop* Drop = dynamic_cast<MatterDrop*>(Matter);
		if (Drop != nullptr)
		{
			Vector4f HLColor;
			HLColor << 0.0f, 0.0f, 0.0f, 1.0f;
			Drop->SetColor(HLColor);
		}
	}

	for (auto &Matter : FoundMatter)
	{
		MatterDrop* Drop = dynamic_cast<MatterDrop*>(Matter);
		if (Drop != nullptr)
		{
			Vector4f HLColor;
			HLColor << 1.0f, 1.0f, 0.0f, 1.0f;
			Drop->SetColor(HLColor);
		}
	}
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
				Matter* TopMost = pWater->Tree->GetTopMost(Selector->Input_Position, 0.3f);
				if (TopMost != nullptr)
				{
					Selector->Select(*TopMost);
				}
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
				Matter* RightMost = pWater->Tree->GetRightMost(Selector->Input_Position, 0.3f);
				if (RightMost != nullptr)
				{
					Selector->Select(*RightMost);
				}
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
				Matter* BottomMost = pWater->Tree->GetBottomMost(Selector->Input_Position, 0.3f);
				if (BottomMost != nullptr)
				{
					Selector->Select(*BottomMost);
				}
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
				Matter* LeftMost = pWater->Tree->GetLeftMost(Selector->Input_Position, 0.3f);
				if (LeftMost != nullptr)
				{
					Selector->Select(*LeftMost);
				}
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