#include "Animation.h"

#include "SceneComponent.h"

Animation::Animation() :
	bLoop(false)
{
}

Animation::~Animation()
{
}

bool Animation::Tick(const double DeltaSeconds)
{
	ElapsedSeconds += DeltaSeconds;
	return false;
}

int Animation::RegisterOutput_Float(float &InProperty)
{
	const int Index = static_cast<int>(OutputMap.size()) - 1;
	Output *NewOutput = new Output();
	NewOutput->Type = EValueType::VALUE_FLOAT;
	NewOutput->ValueFloat = &InProperty;
	OutputMap.insert({ Index, NewOutput });
	return Index;
}

int Animation::RegisterOutput_Vec3(Vector3f &InProperty)
{
	const int Index = static_cast<int>(OutputMap.size()) - 1;
	Output *NewOutput = new Output();
	NewOutput->Type = EValueType::VALUE_VEC3;
	NewOutput->ValueVec3 = &InProperty;
	OutputMap.insert({ Index, NewOutput });
	return Index;
}

int Animation::RegisterOutput_Bool(bool &InProperty)
{
	const int Index = static_cast<int>(OutputMap.size()) - 1;
	Output *NewOutput = new Output();
	NewOutput->Type = EValueType::VALUE_BOOL;
	NewOutput->ValueBool = &InProperty;
	OutputMap.insert({ Index, NewOutput });
	return Index;
}

void Animation::LinkOutput(int OutputIndex, int InputIndex, EOperationType OpType)
{
	OutputLink* Link = new OutputLink();
	Link->OpType = OpType;
	Link->InputIndex = InputIndex;
	OutputMap[OutputIndex]->Links.insert(OutputMap[OutputIndex]->Links.end(), Link);
}

void Animation::UpdateOutputs(SceneComponent& InSceneComponent) const
{
	for (auto &KeyValue : OutputMap)
	{
		Output *CurrentOutput = KeyValue.second;
		InSceneComponent.UpdateInputs(CurrentOutput);
	}
}

void Animation::Reset()
{
	ElapsedSeconds = 0;
}