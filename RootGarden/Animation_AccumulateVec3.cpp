#include "Animation_AccumulateVec3.h"

#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_AccumulateVec3::Animation_AccumulateVec3(Vector3f InVecStart, Vector3f InVecAdd)
	: VecStart(InVecStart), VecAdd(InVecAdd)
{
	OutputIdx_XYZ = RegisterOutput_Vec3(Output_XYZ);
	OutputIdx_X = RegisterOutput_Float(Output_X);
	OutputIdx_Y = RegisterOutput_Float(Output_Y);
	OutputIdx_Z = RegisterOutput_Float(Output_Z);
}

Animation_AccumulateVec3::~Animation_AccumulateVec3()
{
}

bool Animation_AccumulateVec3::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	Output_XYZ += VecAdd * static_cast<float>(DeltaSeconds);
	Output_X = Output_XYZ.x();
	Output_Y = Output_XYZ.y();
	Output_Z = Output_XYZ.z();

	return !Finished();
}

bool Animation_AccumulateVec3::Finished() const
{
	return false;
}

void Animation_AccumulateVec3::Reset()
{
	Animation::Reset();
	VecAdd = Vector3f::Constant(0.0f);
	Output_XYZ = VecStart;
}
