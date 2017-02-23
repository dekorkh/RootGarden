#include "CompOutlineCube.h"
#include "Animation_Lerp2Vec3.h"
#include "MatterWireCube.h"
#include "MatterSolidCube.h"


CompOutlineCube::CompOutlineCube()
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
	
	GLint numAnims = 100;
	Vector3f LookAtStart, LookAtEnd;
	for (GLint coordIdx = 0; coordIdx < 3; coordIdx++)
		LookAtStart(coordIdx) = static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1;

	Vector3f PositionStart, PositionEnd;
	PositionStart(0) = static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1;
	PositionStart(1) = (static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1) * 1.6f;
	PositionStart(2) = static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1;

	for (GLint i = 0; i < numAnims; i++)
	{
		for (GLint coordIdx = 0; coordIdx < 3; coordIdx++)
			LookAtEnd(coordIdx) = static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1;
		
		PositionEnd(0) = static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1;
		PositionEnd(1) = (static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1) * 1.6f;
		PositionEnd(2) = static_cast<float>(rand() % 1000) / 1000.0f * 2 - 1;
		
		float speed = static_cast<float>(rand() % 1000) / 1000.0f * 0.3;

		Animation_Lerp2Vec3 *animA = new Animation_Lerp2Vec3(LookAtStart, LookAtEnd, speed, PositionStart, PositionEnd, speed);
		
		LookAtStart = LookAtEnd;
		PositionStart = PositionEnd;

		animA->LinkOutput(animA->OutputIdx_0XYZ, InputIdx_LookAt, EOperationType::OVERRIDE);
		animA->LinkOutput(animA->OutputIdx_1XYZ, InputIdx_Position, EOperationType::OVERRIDE);

		AddAnimation(animA);
	}

}

CompOutlineCube::~CompOutlineCube()
{
	delete WireCube;
	delete SolidCube;
}

void CompOutlineCube::Build()
{
	SceneComponent::Build();
	WireCube->SetLookAtTarget(Input_LookAt);
	SolidCube->SetLookAtTarget(Input_LookAt);
	Vector3f NewScale;
	NewScale << Input_LookAt(0), Input_LookAt(0), Input_LookAt(0);
	WireCube->SetScale(NewScale * 0.3f);
	SolidCube->SetScale(NewScale * 0.3f);

	WireCube->SetPosition(Input_Position);
	SolidCube->SetPosition(Input_Position);
}