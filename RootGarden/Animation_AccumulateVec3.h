#pragma once

#include "Animation.h"

class Animation_AccumulateVec3 : public Animation
{
public:
	Animation_AccumulateVec3(Vector3f InVecStart, Vector3f InVecAccum);

	~Animation_AccumulateVec3();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	void Reset() override;

	float Output_X;
	float Output_Y;
	float Output_Z;
	Vector3f Output_XYZ;

	int OutputIdx_X;
	int OutputIdx_Y;
	int OutputIdx_Z;
	int OutputIdx_XYZ;

	Vector3f VecAdd;

private:
	Vector3f VecStart;
	
};