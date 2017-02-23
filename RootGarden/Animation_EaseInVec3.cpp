#include "Animation_EaseInVec3.h"

#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_EaseInVec3::Animation_EaseInVec3(Vector3f const &InVecStart, Vector3f const &InVecEnd, float InAlphaPerSecond)
	: 
	VecStart(InVecStart), 
	VecEnd(InVecEnd), 
	AlphaPerSecond(InAlphaPerSecond), 
	Output_XYZ(InVecStart),
	Output_X(InVecStart.x()),
	Output_Y(InVecStart.y()),
	Output_Z(InVecStart.z())
{
	OutputIdx_XYZ = RegisterOutput_Vec3(Output_XYZ);
	OutputIdx_X = RegisterOutput_Float(Output_X);
	OutputIdx_Y = RegisterOutput_Float(Output_Y);
	OutputIdx_Z = RegisterOutput_Float(Output_Z);
}

Animation_EaseInVec3::~Animation_EaseInVec3()
{
}

bool Animation_EaseInVec3::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	float AlphaFactor = (Output_XYZ - VecEnd).norm() / max(StartEndDistance, FLT_EPSILON);
	AlphaFactor = max(AlphaFactor, 0.1f);
	Alpha += static_cast<float>(AlphaPerSecond * AlphaFactor * DeltaSeconds);
	Alpha = Alpha > 1.0f ? 1.0f : Alpha;

	Output_XYZ = VecStart * (1.0f - Alpha) + VecEnd * Alpha;
	Output_X = Output_XYZ.x();
	Output_Y = Output_XYZ.y();
	Output_Z = Output_XYZ.z();

	return !Finished();
}

bool Animation_EaseInVec3::Finished() const
{
	return false;
}

void Animation_EaseInVec3::Reset()
{
	Animation::Reset();
	Alpha = 0.0f;
}

void Animation_EaseInVec3::SetAlphaPerSecond(float InAlphaPerSecond)
{
	AlphaPerSecond = InAlphaPerSecond;
}

void Animation_EaseInVec3::SetVecEnd(Vector3f InVecEnd)
{
	VecStart = Output_XYZ;
	VecEnd = InVecEnd;
	StartEndDistance = (VecStart - VecEnd).norm();
}
