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

Scene_BeanGarden::Scene_BeanGarden() :
	InputType(1)
{
	rect = new MatterRectangle();
	rect->SetScale(Vector3f::Constant(0.01f));

	IncrementVelocityUp		<< 0.0f,  0.02f, 0.0f;
	IncrementVelocityDown	<< 0.0f, -0.02f, 0.0f;
	IncrementVelocityLeft	<< -0.02f, 0.0f, 0.0f;
	IncrementVelocityRight	<< 0.02f, 0.0f, 0.0f;

	pRoot = new MatterCircle();
	pRoot->SetScale(Vector3f::Constant(0.03f));
	AddChild(pRoot);

	Selector = new CompSelector();
	Selector->Select(*pRoot);
	AddChild(Selector);
	
	LastRootTendril = new MatterRootTendril();
	
	AddChild(LastRootTendril);

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
				switch (InputType)
				{
				case 0:
					if (TopMost != nullptr)
					{
						Selector->Select(*TopMost);
					}
					break;
				case 1:

					Selector->AddVelocity(IncrementVelocityUp);
					break;
				}	
			}
		}
		else
		{
			if (UKeyDown)
			{
				UKeyDown = false;

				switch (InputType)
				{
				case 0:
					break;
				case 1:
					Selector->KillVelocity();
					break;
				}
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

				switch (InputType)
				{
				case 0:
					if (RightMost != nullptr)
					{
						Selector->Select(*RightMost);
					}
					break;
				case 1:

					Selector->AddVelocity(IncrementVelocityRight);
					break;
				}
			}
		}
		else
		{
			if (RKeyDown)
			{
				RKeyDown = false;

				switch (InputType)
				{
				case 0:
					break;
				case 1:
					Selector->KillVelocity();
					break;
				}
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

				switch (InputType)
				{
				case 0:
					if (BottomMost != nullptr)
					{
						Selector->Select(*BottomMost);
					}
					break;
				case 1:

					Selector->AddVelocity(IncrementVelocityDown);
					break;
				}
			}
		}
		else
		{
			if (DKeyDown)
			{
				DKeyDown = false;

				switch (InputType)
				{
				case 0:
					break;
				case 1:
					Selector->KillVelocity();
					break;
				}
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

				switch (InputType)
				{
				case 0:
					if (LeftMost != nullptr)
					{
						Selector->Select(*LeftMost);
					}
					break;
				case 1:

					Selector->AddVelocity(IncrementVelocityLeft);
					break;
				}
			}
		}
		else
		{
			if (LKeyDown)
			{
				LKeyDown = false;

				switch (InputType)
				{
				case 0:
					break;
				case 1:
					Selector->KillVelocity();
					break;
				}
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
		LastRootSegment = LastRootTendril->SpawnRoot(LastRootSegment, Selector->Input_Position);
		
		
		rect->SetPosition(Selector->Input_Position);
		AddChild(rect);
		break;
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}