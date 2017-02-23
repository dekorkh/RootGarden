#pragma once

#include "Animation.h"

class Animation_EaseInVec3 : public Animation
{
public:
	Animation_EaseInVec3(Vector3f const &InVecStart, Vector3f const &InVecEnd, float InAlphaPerSecond);

	~Animation_EaseInVec3();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	void Reset() override;

	void SetAlphaPerSecond(float InAlphaPerSecond);
	void SetVecEnd(Vector3f InVecEnd);

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
	float StartEndDistance;
};