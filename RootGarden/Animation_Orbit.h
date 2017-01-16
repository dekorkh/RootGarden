#pragma once

#include "Animation.h"

class Matter;

class Animation_Orbit : public Animation
{
public:
	Animation_Orbit();
	~Animation_Orbit();

	bool Tick(const double DeltaSeconds) override;

	Vector3f Out;
	int OutputIdx_Out; 
};