#pragma once

#include "SceneComponent.h"

class MatterRootCluster : public SceneComponent
{
public:
	MatterRootCluster();

private:
	void SpawnTendril();

	int NumTendrils;
	const int MaxTendrils;

	bool bGrown;
};