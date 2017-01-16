#pragma once

#include "Animation.h"
#include "Eigen/Dense"

using namespace Eigen;

class MatterRootSegment;

class Animation_GrowTo : public Animation
{
public:
	Animation_GrowTo(const MatterRootSegment* pRootSegment, float InGrams, Vector3f InTarget);
	~Animation_GrowTo();

	void SetRate(float GramsPerSecond);

	// Returns false when finished to signal animation list to play next animation.
	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	float Out;
	int OutputIdx_Out;

private:
	const MatterRootSegment* pRootSegment;
	float MassRemaining;
	float Rate;
	Vector3f Target;
	bool bReachedDestination;
	bool bRanOutOfMass;
};

