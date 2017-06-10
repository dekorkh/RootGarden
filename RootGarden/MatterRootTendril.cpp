#include "MatterRootTendril.h"
#include "MatterRootSegment.h"
#include "MatterRectangle.h"
#include "Animation_GrowTo.h"
#include "Animation_Trigger.h"
#include "Animation_Aim.h"

MatterRootTendril::MatterRootTendril() :
	SceneComponent("MatterRootTendril", Counter<MatterRootTendril>::Count()),
	NumSegments(0),
	MaxSegments(8)
{
	/*
	Vector2f v1 = Vector2f();
	v1 << -1.0f, 0.0f;
	
	Vector2f v2 = Vector2f();
	v2 << -1.0f, -1.0f;

	Vector2f v3 = Vector2f();
	v3 << 1.0f, -1.0f;

	Vector2f v4 = Vector2f();
	v4 << 1.0f, 1.0f;
	
	Vector2f v5 = Vector2f();
	v5 << -1.0f, 1.0f;

	float f = 0.5f;

	Targets.emplace_back(v1);
	Targets.emplace_back(v2);
	Targets.emplace_back(v3);
	Targets.emplace_back(v4);
	Targets.emplace_back(v5);
	*/
}

void MatterRootTendril::Tick(double DeltaSeconds)
{
	SceneComponent::Tick(DeltaSeconds); //Ticks children
}

MatterRootSegment* MatterRootTendril::SpawnRoot(const MatterRootSegment* SourceSegment, const Vector3f& Target)
{
	NumSegments++;
	MatterRootSegment* newRootSegment = new MatterRootSegment();

	if (SourceSegment != nullptr)
	{
		newRootSegment->SetSourceRootSegment(*SourceSegment);
	}	
	
	int InputIdx_LookAt = newRootSegment->RegisterInput_Vec3(newRootSegment->Param_Target);
	newRootSegment->InputMap[InputIdx_LookAt]->InputEffects.bComponentParameters = true;
	Animation_Aim* animAim = new Animation_Aim(Target - newRootSegment->Transform.GetPosition());
	animAim->LinkOutput(animAim->OutputIdx_AimVector, InputIdx_LookAt, EOperationType::OVERRIDE);
	newRootSegment->AddAnimation(animAim);
	
	int InputIdx_MassCapacity = newRootSegment->RegisterInput_Float(newRootSegment->MassCapacity);
	newRootSegment->InputMap[InputIdx_MassCapacity]->InputEffects.bVertPositions = true;
	newRootSegment->InputMap[InputIdx_MassCapacity]->InputEffects.bComponentParameters = true;
	Animation_GrowTo* newRootAnim = new Animation_GrowTo(newRootSegment, 0.5f, Target);

	newRootAnim->SetRate(0.003f);
	newRootAnim->LinkOutput(newRootAnim->OutputIdx_Out, InputIdx_MassCapacity, EOperationType::ACCUMULATE);
	newRootSegment->AddAnimation(newRootAnim);

	int InputIdx_bGrown = newRootSegment->RegisterInput_Bool(newRootSegment->bGrown);
	newRootSegment->InputMap[InputIdx_bGrown]->InputEffects.bComponentParameters = true;
	Animation_Trigger* animTriggerGrown = new Animation_Trigger();
	animTriggerGrown->LinkOutput(animTriggerGrown->OutputIdx_Out, InputIdx_bGrown, EOperationType::OVERRIDE);
	newRootSegment->AddAnimation(animTriggerGrown);

	AddChild(newRootSegment);
	return newRootSegment;
}