#pragma once

#include "Animation.h"

class Animation_LerpVec3 : public Animation
{
public:
	Animation_LerpVec3(Vector3f InVecStart, Vector3f InVecEnd, float InAlphaPerSecond);

	~Animation_LerpVec3();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	void Reset() override;

	void SetAlphaPerSecond(float InAlphaPerSecond);

	float Output_X;
	float Output_Y;
	float Output_Z;
	Vector3f Output_XYZ;

	int OutputIdx_X;
	int OutputIdx_Y;
	int OutputIdx_Z;
	int OutputIdx_XYZ;

private:
	Vector3f VecStart;
	Vector3f VecEnd;
	float Alpha;
	float AlphaPerSecond;
};