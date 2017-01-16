#pragma once

#include "Animation.h"

class Animation_FrameTime : public Animation
{
public:
	/* Animation_FrameTime
	/	This animation will return a 0-1 value representing a fraction 
	/	of time passed since last frame by the specified MaxTime (in seconds)
	/	and smoothed by RunningAverage number of frames.
	*/
	Animation_FrameTime(double InMaxTime, int InRunningAverage = 0);

	~Animation_FrameTime();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	float Out;
	int OutputIdx_Out;

private:
	/* Every frame's delta time will be devided by this amount returning its fraction. */
	double MaxTime;
	
	/* It just can't stop. */
	int RunningAverage;
};

