#pragma once

#include "Animation.h"

class Animation_Aim : public Animation
{
public:
	Animation_Aim(Vector3f Target);
	~Animation_Aim();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	Vector3f Out_AimVector;
	int OutputIdx_AimVector;
};