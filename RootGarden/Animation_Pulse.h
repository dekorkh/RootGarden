#pragma once

#include "Animation.h"

class Animation_Pulse : public Animation
{
public:
	Animation_Pulse();
	Animation_Pulse(float InPulseStrength);

	~Animation_Pulse();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	float Out;
	int OutputIdx_Out;
};

