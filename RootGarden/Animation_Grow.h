#pragma once

#pragma once

#include "Animation.h"

class Animation_Grow : public Animation
{
public:
	Animation_Grow();
	Animation_Grow(float InGrams);

	~Animation_Grow();

	void SetRate(float GramsPerSecond);

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	float Out;
	int OutputIdx_Out;

private:
	float Amount;
	float Rate;
};

