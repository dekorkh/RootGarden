#include "Animation_AccumulateVec3.h"

#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_AccumulateVec3::Animation_AccumulateVec3(Vector3f InVecStart, Vector3f InVecAdd, AccumulateFunction InCurve)
	: VecStart(InVecStart), VecAdd(InVecAdd), Curve(InCurve)
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

	switch (Curve)
	{
	case AccumulateFunction::Linear:
		break;
	case AccumulateFunction::Quadratic:
		VecAdd += VecAdd * static_cast<float>(DeltaSeconds);
		break;
	case AccumulateFunction::Cubic:
		VecAdd += VecAdd * static_cast<float>(DeltaSeconds);
		VecAdd += VecAdd * static_cast<float>(DeltaSeconds);
		break;
	}

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

void Animation_AccumulateVec3::SetDelta(Vector3f &InDelta)
{
	VecAdd = InDelta;
}

Vector3f Animation_AccumulateVec3::GetDelta() const
{
	return VecAdd;
}