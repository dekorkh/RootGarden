#pragma once

#include "Animation.h"

class Animation_AimOrbit : public Animation
{
public:
	Animation_AimOrbit();
	~Animation_AimOrbit();

	bool Tick(const double DeltaSeconds) override;

	void SetOrbitSpeed(float OrbitSpeedIn);

	Vector3f Out;
	int OutputIdx_Out;

private:
	float OrbitSpeed;
};

