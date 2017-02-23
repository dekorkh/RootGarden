#include "CompTravelCube.h"
#include "Animation_AccumulateVec3.h"
#include "MatterWireCube.h"
#include "MatterSolidCube.h"
#include "MatterRing.h"


CompTravelCube::CompTravelCube()
{
	WireCube = new MatterWireCube();
	SolidCube = new MatterSolidCube();

	float PosX = static_cast<float>(rand() % 1920) / 1920.0f * 1.6f * 2 - 1;
	float PosY = static_cast<float>(rand() % 1920) / 1920.0f * 1.0f * 2 - 1;
	Vector3f NewPos;
	NewPos << PosX, PosY, 0.0f;
	WireCube->SetPosition(NewPos);
	SolidCube->SetPosition(NewPos);

	float ScaleFactor = static_cast<float>(rand() % 1000) / 1000.0f * 0.4f;
	Vector3f NewScale;
	NewScale << ScaleFactor, ScaleFactor, ScaleFactor;
	WireCube->SetScale(NewScale);
	SolidCube->SetScale(NewScale);
	SolidCube->bIsOcclusion = true;

	AddChild(SolidCube);
	AddChild(WireCube);

	GLint InputIdx_LookAt = RegisterInput_Vec3(Input_LookAt);
	this->InputMap[InputIdx_LookAt]->InputEffects.bComponentParameters = true;

	GLint InputIdx_Position = RegisterInput_Vec3(Input_Position);
	this->InputMap[InputIdx_Position]->InputEffects.bComponentParameters = true;

	Vector3f VecInit, VecAdd;
	VecInit = Vector3f::Constant(0.0f);
	VecAdd = Vector3f::Constant(0.0f);
	
	VelocityAnim = new Animation_AccumulateVec3(VecInit, VecAdd);
	VelocityAnim->LinkOutput(VelocityAnim->OutputIdx_XYZ, InputIdx_Position, EOperationType::OVERRIDE);
	AddAnimation(VelocityAnim);
}

CompTravelCube::~CompTravelCube()
{
	delete WireCube;
	delete SolidCube;
	delete VelocityAnim;
}

void CompTravelCube::Build()
{
	SceneComponent::Build();
	SolidCube->SetPosition(Input_Position);
	WireCube->SetPosition(Input_Position);

	SolidCube->SetLookAt(VelocityAnim->VecAdd);
	WireCube->SetLookAt(VelocityAnim->VecAdd);
}

void CompTravelCube::AddVelocity(Vector3f Velocity)
{
	VelocityAnim->VecAdd += Velocity;
}