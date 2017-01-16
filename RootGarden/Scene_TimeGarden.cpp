#include "Scene_TimeGarden.h"
#include "Animation_Orbit.h"
#include "Animation_AimOrbit.h"
#include "Animation_Aim.h"
#include "Animation_Blank.h"
#include "Animation_Grow.h"

#define _USE_MATH_DEFINES
#include "math.h"

Scene_TimeGarden::Scene_TimeGarden()
{
	Circle = new MatterCircle();
	Circle->SetScale(Vector3f::Constant(0.01f));
	Vector3f NewPosition = Vector3f::Constant(0.0f);
	Circle->SetPosition(NewPosition);
	Vector4f ColorInner;
	ColorInner << 1.0, 1.0, 0.0, 1.0f;
	Vector4f ColorOuter;
	ColorOuter << 1.0f, 0.0f, 0.0f, 0.0f;
	Circle->SetColors(ColorInner, ColorOuter);
	AddChild(Circle);

	Animation* CircleOrbit = new Animation_Orbit();
	Circle->AddAnimation(CircleOrbit);

	
	pMatterRootSegment = new MatterRootSegment();
	Animation_Grow* AnimGrow = new Animation_Grow(0.1f);
	AnimGrow->SetRate(0.01f);
	pMatterRootSegment->AddAnimation(AnimGrow);
	Vector3f TestLookAt = Vector3f::Constant(0.0f);
	int r1 = rand();
	int r2 = rand();
	//TestLookAt(0) = static_cast<float>(r1 % 1000000) / 500000 - 0.5f;
	//TestLookAt(1) = static_cast<float>(r2 % 1000000) / 500000 - 0.5f;
	
	TestLookAt(0) = 0.2f;
	TestLookAt(1) = -0.2f;

	pMatterRootSegment->SetLookAt(TestLookAt);

	Animation* Anim_Blank = new Animation_Blank();
	pMatterRootSegment->AddAnimation(Anim_Blank);

	pMatterRootSegment->SelectAnimation(0);

	AddChild(pMatterRootSegment);

	//another root segment
	MatterRootSegment *rootsegment2 = new MatterRootSegment();
	Animation_Grow* AnimGrow2 = new Animation_Grow(0.1f);
	AnimGrow2->SetRate(0.01f);
	rootsegment2->AddAnimation(AnimGrow2);
	Vector3f TestLookAt2 = Vector3f::Constant(0.0f);

	TestLookAt2(0) = -0.2f;
	TestLookAt2(1) = 0.2f;

	rootsegment2->SetLookAt(TestLookAt2);

	rootsegment2->SelectAnimation(0);

	pMatterRootSegment->AddChild(rootsegment2);

	MatterRootTendril *tendril = new MatterRootTendril();

	AddChild(tendril);
}


Scene_TimeGarden::~Scene_TimeGarden()
{
}

void Scene_TimeGarden::Tick(double DeltaSeconds)
{
	Scene::Tick(DeltaSeconds);
	pMatterRootSegment = new MatterRootSegment();
	Animation_Grow* AnimGrow = new Animation_Grow(0.1f);
	AnimGrow->SetRate(0.01f);
	pMatterRootSegment->AddAnimation(AnimGrow);
	Vector3f TestLookAt = Vector3f::Constant(0.0f);
	int r1 = rand();
	int r2 = rand();
	TestLookAt(0) = static_cast<float>(r1 % 1000000) / 500000 - 0.5f;
	TestLookAt(1) = static_cast<float>(r2 % 1000000) / 500000 - 0.5f;
}

void Scene_TimeGarden::HandleInput(int Key, int x, int y, bool down)
{
	switch (Key)
	{
	case 102:	//right
		if (down)
		{
			if (!RKeyDown)
			{
				pMatterRootSegment->SelectAnimation(0);
				RKeyDown = true;
			}
		}
		else
		{
			if (RKeyDown)
			{
				pMatterRootSegment->SelectAnimation(2);
				RKeyDown = false;
			}
		}
		break;
	case 100: //left
		if (down)
		{
			if (!LKeyDown)
			{
				pMatterRootSegment->SelectAnimation(1);
				LKeyDown = true;
			}
		}
		else
		{
			if (LKeyDown)
			{
				pMatterRootSegment->SelectAnimation(2);
				LKeyDown = false;
			}
		}
		break;
	case 27: //left
		if (down && !EscDown)
		{
			exit(0);
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