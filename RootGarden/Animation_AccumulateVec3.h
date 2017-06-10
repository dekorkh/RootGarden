#pragma once

#include "Animation.h"

enum AccumulateFunction
{
	Linear = 0,
	Quadratic,
	Cubic
};

class Animation_AccumulateVec3 : public Animation
{
public:
	Animation_AccumulateVec3(Vector3f InVecStart, Vector3f InVecAccum, AccumulateFunction InCurve = AccumulateFunction::Linear);

	~Animation_AccumulateVec3();

	bool Tick(const double DeltaSeconds) override;

	void SetDelta(Vector3f &InDelta);
	Vector3f GetDelta() const;
	

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

	AccumulateFunction Curve;

private:
	Vector3f VecStart;
	
};