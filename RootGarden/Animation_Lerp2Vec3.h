#pragma once

#include "Animation.h"

class Animation_Lerp2Vec3 : public Animation
{
public:
	Animation_Lerp2Vec3(Vector3f InVecStart0, Vector3f InVecEnd0, float InAlphaPerSecond0, 
						Vector3f InVecStart1, Vector3f InVecEnd1, float InAlphaPerSecond1);

	~Animation_Lerp2Vec3();

	bool Tick(const double DeltaSeconds) override;

	bool Finished() const override;

	void Reset() override;

	void SetAlphaPerSecond(float InAlphaPerSecond0, float InAlphaPerSecond1);

	float Output_0X;
	float Output_0Y;
	float Output_0Z;
	Vector3f Output_0XYZ;

	int OutputIdx_0X;
	int OutputIdx_0Y;
	int OutputIdx_0Z;
	int OutputIdx_0XYZ;

	float Output_1X;
	float Output_1Y;
	float Output_1Z;
	Vector3f Output_1XYZ;

	int OutputIdx_1X;
	int OutputIdx_1Y;
	int OutputIdx_1Z;
	int OutputIdx_1XYZ;

private:
	Vector3f VecStart0;
	Vector3f VecEnd0;
	float Alpha0;
	float AlphaPerSecond0;

	Vector3f VecStart1;
	Vector3f VecEnd1;
	float Alpha1;
	float AlphaPerSecond1;
};