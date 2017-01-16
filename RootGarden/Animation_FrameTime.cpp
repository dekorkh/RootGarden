#include "Animation_FrameTime.h"

#include "Matter.h"
#include "Eigen/Dense"

#define _USE_MATH_DEFINES
#include "math.h"

using namespace Eigen;

Animation_FrameTime::Animation_FrameTime(double InMaxTime, int InRunningAverage) :
	MaxTime(max(InMaxTime, DBL_EPSILON)),
	Out(0.0f),
	RunningAverage(InRunningAverage)
{
	OutputIdx_Out = RegisterOutput_Float(Out);
}

Animation_FrameTime::~Animation_FrameTime()
{
}

bool Animation_FrameTime::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	float OutDelta = static_cast<float>(fmin((DeltaSeconds / MaxTime), 1.0)) - Out;
	Out += OutDelta * (1.0f / RunningAverage);

	return !Finished();
}

bool Animation_FrameTime::Finished() const
{
	return false;
}