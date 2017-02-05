#pragma once

#include "SceneComponent.h"
#include <deque>

class MatterDrop;
class Animation_Trigger;
class Animation_LerpVec3;

class CompWater : public SceneComponent
{
public:
	CompWater();

	void Build() override;

	/* SetMaxDrops
	Set the number of max drops on field
	*/
	void SetMaxDrops(int Num);

	/* SetSpawnRate
	Set the number of drops to spawn per second
	*/
	void SetSpawnRate(float Num);

	/* SetBounds
	Set the width and height of the bounds.
	*/
	void SetBounds(float half_width, float half_height);

	void Tick(double DeltaSeconds) override;

	bool bDoSpawn;

private:
	vector<MatterDrop*> WaterDrops;
	
	float SpawnPerSecond;
	int NumDrops;
	int MaxDrops;
	float BoundsWidth;
	float BoundsHeight;

	Animation_Trigger* pAnimTrigger;
	Animation_LerpVec3* pAnimLerp;
}; 
