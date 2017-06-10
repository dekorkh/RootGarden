#include "Animation_GrowTo.h"
#include "MatterRectangle.h"
#include "Matter.h"
#include "Eigen/Dense"
#include "MatterRootSegment.h"

using namespace Eigen;

Animation_GrowTo::Animation_GrowTo(const MatterRootSegment* pRootSegment,
	float InGrams,
	Vector3f InTarget) :
	pRootSegment(pRootSegment),
	MassRemaining(InGrams),
	Target(InTarget),
	Out(0.0f),
	OutputIdx_Out(0)
{
	OutputIdx_Out = RegisterOutput_Float(Out);
}

Animation_GrowTo::~Animation_GrowTo()
{
}

bool Animation_GrowTo::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);
	
	//Each tick the mass to grow by is the lesser of massAtRate, massRemaining, massToTarget
	//if the remainingMass is the lesser, the remainingMass should be set explicitely to 0.0f after

	float MassAtRate = Rate * static_cast<float>(DeltaSeconds);

	float MassToTarget = pRootSegment->MassCostToTarget(Target);
	
	float MassGrow = 0.0f;

	if (MassAtRate <= MassToTarget && MassAtRate <= MassRemaining)
	{
		MassRemaining -= MassAtRate;
		MassGrow = MassAtRate;
	}
	else if (MassToTarget <= MassAtRate && MassToTarget <= MassRemaining)
	{
		MassRemaining -= MassToTarget;
		MassGrow = MassToTarget;
		bReachedDestination = true;
	}
	else if (MassRemaining <= MassToTarget && MassRemaining <= MassAtRate)
	{
		MassGrow = MassRemaining;
		MassRemaining = 0.0f;
		bRanOutOfMass = true;
	}

	if (MassGrow > 0.0f)
	{
		Out = MassGrow;
	}
	
	return !Finished();
}

void Animation_GrowTo::SetRate(float GramsPerSecond)
{
	Rate = GramsPerSecond;
}

bool Animation_GrowTo::Finished() const
{
	return (bReachedDestination || bRanOutOfMass);
}