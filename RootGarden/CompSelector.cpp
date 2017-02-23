#include "CompSelector.h"
#include "Animation_EaseInVec3.h"
#include "MatterCircle.h"
#include "MatterSolidCube.h"
#include "MatterRing.h"


CompSelector::CompSelector()
{
	Outline = new MatterCircle();
	Outline->Mesh->MatterType = GL_POINTS;
	Vector4f NewColor;
	NewColor << 1.0f, 0.8f, 0.3f, 1.0f;
	Outline->SetColors(NewColor, NewColor);
	
	Vector3f NewScale;
	NewScale << 0.3f, 0.3f, 0.3f;
	Outline->SetScale(NewScale);

	AddChild(Outline);

	
	GLint InputIdx_LookAt = RegisterInput_Vec3(Input_LookAt);
	this->InputMap[InputIdx_LookAt]->InputEffects.bComponentParameters = true;

	GLint InputIdx_Position = RegisterInput_Vec3(Input_Position);
	this->InputMap[InputIdx_Position]->InputEffects.bComponentParameters = true;

	Vector3f VecInit, VecDest;
	VecInit = Vector3f::Constant(0.0f);
	VecDest = Vector3f::Constant(0.0f);

	VelocityAnim = new Animation_EaseInVec3(VecInit, VecDest, 0.5f);
	VelocityAnim->LinkOutput(VelocityAnim->OutputIdx_XYZ, InputIdx_Position, EOperationType::OVERRIDE);
	AddAnimation(VelocityAnim);
	
}

CompSelector::~CompSelector()
{
	delete Outline;
	delete VelocityAnim;
}

void CompSelector::Build()
{
	SceneComponent::Build();
	Outline->SetPosition(Input_Position);
}

void CompSelector::GoToPosition(Vector3f InPosition)
{
	VelocityAnim->SetVecEnd(InPosition);
}

void CompSelector::Select(Matter const &Target)
{
	Selected = &Target;
	GoToPosition(Target.Transform.GetPosition());
}

void CompSelector::SelectLeft()
{

}
void CompSelector::SelectRight()
{

}

void CompSelector::SelectUp()
{

}

void CompSelector::SelectDown()
{

}