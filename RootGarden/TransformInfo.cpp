#include "TransformInfo.h"
#include "GameStats.h"
#include "time.h"

TransformInfo::TransformInfo() : 
	Scale(Vector3f::Constant(1.0f)), 
	Position(Vector3f::Constant(0.0f)), 
	LookAt(Vector3f::UnitX())
{
	// Last row of the model matrix is constant
	ModelMatrix(12) = 0.0f;
	ModelMatrix(13) = 0.0f;
	ModelMatrix(14) = 0.0f;
	ModelMatrix(15) = 1.0f;
}

TransformInfo::~TransformInfo()
{
}

void TransformInfo::ComputeModelMatrix()
{
	Vector3f WorldUp = Vector3f::UnitY();
	Vector3f Depth = WorldUp.cross(LookAt).normalized();
	Vector3f LocalUp = LookAt.cross(Depth).normalized();

	ModelMatrix.block<1, 3>(0, 0) = LookAt * Scale(0);
	ModelMatrix.block<1, 3>(1, 0) = LocalUp * Scale(1);
	ModelMatrix.block<1, 3>(2, 0) = Depth * Scale(2);

	ModelMatrix(3) = Position(0);
	ModelMatrix(7) = Position(1);
	ModelMatrix(11) = Position(2);
}

void TransformInfo::SetUniform(ShaderProgram const * InShaderProgram) const
{
	return InShaderProgram->SetUniform("ModelMatrix", ModelMatrix.data());
}

void TransformInfo::SetScale(const Vector3f& InScale)
{
	Scale = InScale;
}

void TransformInfo::SetPosition(const Vector3f& InPosition)
{
	Position = InPosition;
}

void TransformInfo::SetLookAt(const Vector3f& InLookAt)
{
	LookAt = InLookAt.normalized();
}

void TransformInfo::AddRotation(const float Deg)
{
	float newRotation = ConvertLookAtToDeg(LookAt) + Deg;
	Vector3f newLookAt = Vector3f::Constant(0.0f);
	ConvertDegToLookAt(newRotation, newLookAt);
	SetLookAt(newLookAt);
}

void TransformInfo::ConvertDegToLookAt(const float degrees, Vector3f& OutLookAt)
{
	float degCorrected = degrees < 180.0f ? degrees : (degrees - 180.0f) * -1.0f;
	float Rads = degCorrected * (static_cast<float>(M_PI) / 180.0f);
	OutLookAt(0) = cos(Rads);
	OutLookAt(1) = sin(Rads);
	OutLookAt(2) = 0.0f;
}

float TransformInfo::ConvertLookAtToDeg(const Vector3f& OutLookAt)
{
	float deg = ((atan2f(OutLookAt(1), OutLookAt(0)) / static_cast<float>(M_PI)) * 180.0f);
	deg = deg > 0.0f ? deg : 180 + (deg * -1);
	return deg;
}

Vector3f TransformInfo::GetLookAt() const
{
	return LookAt;
}

Vector3f TransformInfo::GetPosition() const
{
	return Position;
}