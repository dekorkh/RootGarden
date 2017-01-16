#include "Animation_Trigger.h"

#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_Trigger::Animation_Trigger(bool bStartState)
	: Out(0.0f), bLastState(bStartState)
{
	OutputIdx_Out = RegisterOutput_Bool(Out);
}

Animation_Trigger::~Animation_Trigger()
{
}

bool Animation_Trigger::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	Out = !bLastState;
	bLastState = Out;

	return !Finished();
}

bool Animation_Trigger::Finished() const
{
	return true;
}