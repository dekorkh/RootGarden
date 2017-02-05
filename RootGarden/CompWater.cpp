#include "CompWater.h"
#include "MatterDrop.h"
#include "Animation_Trigger.h"
#include "Animation_LerpVec3.h"

CompWater::CompWater()
	:
	MaxDrops(0),
	NumDrops(0),
	BoundsWidth(1.6f),
	BoundsHeight(1.0f),
	bDoSpawn(false),
	SpawnPerSecond(1.0f)
{
	pAnimLerp = new Animation_LerpVec3(Vector3f::Constant(0.0f), Vector3f::Constant(1.0f), 1.0f * SpawnPerSecond);
	AddAnimation(pAnimLerp);

	pAnimTrigger = new Animation_Trigger();
	int InputIdx_bDoSpawn = RegisterInput_Bool(bDoSpawn);
	pAnimTrigger->LinkOutput(pAnimTrigger->OutputIdx_Out, InputIdx_bDoSpawn, EOperationType::ACCUMULATE);
	AddAnimation(pAnimTrigger);
}

void CompWater::Build()
{
	SceneComponent::Build();
}

void CompWater::SetMaxDrops(int Num)
{
	MaxDrops = Num;
}

void CompWater::SetSpawnRate(float Num)
{
	SpawnPerSecond = Num;
	pAnimLerp->SetAlphaPerSecond(1.0f * SpawnPerSecond);
}

void CompWater::SetBounds(float half_width, float half_height)
{
	BoundsWidth = half_width;
	BoundsHeight = half_height;
}

void CompWater::Tick(double DeltaSeconds)
{
	SceneComponent::Tick(DeltaSeconds);
	if (bDoSpawn && NumDrops < MaxDrops)
	{
		NumDrops++;

		srand(NumDrops * 1000);
		float x = static_cast<float>(rand() % 1000) / 1000 * 2 * BoundsWidth - BoundsWidth;
		float y = static_cast<float>(rand() % 1000) / 1000 * 2 * BoundsHeight - BoundsHeight;

		MatterDrop* pDrop = new MatterDrop();
		Vector3f vecPos;
		vecPos << x, y, 0.0f;
		pDrop->SetPosition(vecPos);
		WaterDrops.push_back(pDrop);
		AddChild(pDrop);

		bDoSpawn = false;
		pAnimTrigger->Reset();
		pAnimLerp->Reset();
		SelectAnimation(0);
	}
}