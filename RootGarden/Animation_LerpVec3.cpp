#include "Animation_LerpVec3.h"

#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_LerpVec3::Animation_LerpVec3(Vector3f InVecStart, Vector3f InVecEnd, float InAlphaPerSecond)
	: VecStart(InVecStart), VecEnd(InVecEnd), AlphaPerSecond(InAlphaPerSecond)
{
	OutputIdx_XYZ = RegisterOutput_Vec3(Output_XYZ);
	OutputIdx_X = RegisterOutput_Float(Output_X);
	OutputIdx_Y = RegisterOutput_Float(Output_Y);
	OutputIdx_Z = RegisterOutput_Float(Output_Z);
}

Animation_LerpVec3::~Animation_LerpVec3()
{
}

bool Animation_LerpVec3::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	Alpha += AlphaPerSecond * DeltaSeconds;
	Alpha = Alpha > 1.0f ? 1.0f : Alpha;

	Output_XYZ = VecStart * (1.0f - Alpha) + VecEnd * Alpha;
	Output_X = Output_XYZ.x();
	Output_Y = Output_XYZ.y();
	Output_Z= Output_XYZ.z();

	return !Finished();
}

bool Animation_LerpVec3::Finished() const
{
	return Alpha >= 1.0f;
}

void Animation_LerpVec3::Reset()
{
	Animation::Reset();
	Alpha = 0.0f;
}

void Animation_LerpVec3::SetAlphaPerSecond(float InAlphaPerSecond)
{
	AlphaPerSecond = InAlphaPerSecond;
}
