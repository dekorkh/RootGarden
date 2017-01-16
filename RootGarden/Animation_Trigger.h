#pragma once

#include "Animation.h"

class Animation_Trigger : public Animation
{
public:
	Animation_Trigger(bool bStartState = false);

	~Animation_Trigger();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	bool Out;
	int OutputIdx_Out;

private:
	bool bLastState;
};

