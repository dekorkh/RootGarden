#include "Animation_Lerp2Vec3.h"

#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_Lerp2Vec3::Animation_Lerp2Vec3(
	Vector3f InVecStart0, Vector3f InVecEnd0, float InAlphaPerSecond0,
	Vector3f InVecStart1, Vector3f InVecEnd1, float InAlphaPerSecond1)
	: 
	VecStart0(InVecStart0), VecEnd0(InVecEnd0), AlphaPerSecond0(InAlphaPerSecond0),
	VecStart1(InVecStart1), VecEnd1(InVecEnd1), AlphaPerSecond1(InAlphaPerSecond1)
{
	OutputIdx_0XYZ = RegisterOutput_Vec3(Output_0XYZ);
	OutputIdx_0X = RegisterOutput_Float(Output_0X);
	OutputIdx_0Y = RegisterOutput_Float(Output_0Y);
	OutputIdx_0Z = RegisterOutput_Float(Output_0Z);

	OutputIdx_1XYZ = RegisterOutput_Vec3(Output_1XYZ);
	OutputIdx_1X = RegisterOutput_Float(Output_1X);
	OutputIdx_1Y = RegisterOutput_Float(Output_1Y);
	OutputIdx_1Z = RegisterOutput_Float(Output_1Z);
}

Animation_Lerp2Vec3::~Animation_Lerp2Vec3()
{
}

bool Animation_Lerp2Vec3::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	Alpha0 += static_cast<float>(AlphaPerSecond0 * DeltaSeconds);
	Alpha0 = Alpha0 > 1.0f ? 1.0f : Alpha0;

	Output_0XYZ = VecStart0 * (1.0f - Alpha0) + VecEnd0 * Alpha0;
	Output_0X = Output_0XYZ.x();
	Output_0Y = Output_0XYZ.y();
	Output_0Z = Output_0XYZ.z();

	Alpha1 += static_cast<float>(AlphaPerSecond1 * DeltaSeconds);
	Alpha1 = Alpha1 > 1.0f ? 1.0f : Alpha1;

	Output_1XYZ = VecStart1 * (1.0f - Alpha1) + VecEnd1 * Alpha1;
	Output_1X = Output_1XYZ.x();
	Output_1Y = Output_1XYZ.y();
	Output_1Z = Output_1XYZ.z();

	return !Finished();
}

bool Animation_Lerp2Vec3::Finished() const
{
	return (Alpha0 >= 1.0f && Alpha1 >= 1.0f);
}

void Animation_Lerp2Vec3::Reset()
{
	Animation::Reset();
	Alpha0 = 0.0f;
	Alpha1 = 0.0f;
}

void Animation_Lerp2Vec3::SetAlphaPerSecond(float InAlphaPerSecond0, float InAlphaPerSecond1)
{
	AlphaPerSecond0 = InAlphaPerSecond0;
	AlphaPerSecond1 = InAlphaPerSecond1;
}
