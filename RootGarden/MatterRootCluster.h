#pragma once

#include "SceneComponent.h"

class MatterRootCluster : public SceneComponent
{
public:
	MatterRootCluster();

	void Tick(double DeltaSeconds) override;

private:
	void SpawnTendril();

	int NumTendrils;
	const int MaxTendrils;

	bool bGrown;
};