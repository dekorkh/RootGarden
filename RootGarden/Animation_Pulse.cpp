#include "Animation_Pulse.h"

#include "Matter.h"
#include "Eigen/Dense"
#include "MatterRootSegment.h"
#include "MatterRectangleGradient.h"

#define _USE_MATH_DEFINES
#include "math.h"

using namespace Eigen;

Animation_Pulse::Animation_Pulse()
{
}

Animation_Pulse::Animation_Pulse(float InPulseStrength)
	: Out(0.0f)
{
	OutputIdx_Out = RegisterOutput_Float(Out);
}

Animation_Pulse::~Animation_Pulse()
{
}

bool Animation_Pulse::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	Out = max(static_cast<float>(-1.0f * tan((ElapsedSeconds + 1.0f) * 3.7f)), 0.0f);
	
	return !Finished();
}

bool Animation_Pulse::Finished() const
{
	return ElapsedSeconds > 30.0f; // / 3.7f * 10;
}