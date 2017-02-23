#include "SceneComponent.h"
#include "GlobalState.h"

vector<SceneComponent*> SceneComponent::AllPrims;
vector<GLuint> SceneComponent::OcclusionQueries;

SceneComponent::SceneComponent() : bDirtyComponentParameters(true)
{
	Initialize();
}

SceneComponent::SceneComponent(const string& InName, const int InSerial) : bDirtyComponentParameters(true)
{
	//Create a unique name using the serial number
	Name = InName + "_";
	string Serial = to_string(InSerial);
	for (unsigned int i = 0; i < (4 - Serial.length()); i++)
		Name += "0";
	Name += Serial;

	Initialize();
}

void SceneComponent::Initialize()
{
	bTickEnabled = true;
	AllPrims.push_back(this); // Add self to all prims
	pAnimationList = new AnimationList(*this);
}

SceneComponent::~SceneComponent()
{
	delete pAnimationList;
}

void SceneComponent::AddChild(SceneComponent* Child)
{
	Children.insert(Children.end(), Child);
}

void SceneComponent::HandleInput(int Key, int x, int y, bool down)
{
}

void SceneComponent::GatherChildren(vector<SceneComponent*>* OutChildren)
{
	OutChildren->insert(OutChildren->end(), this);
	for (SceneComponent* Child : Children)
	{
		Child->GatherChildren(OutChildren);
	}
}

void SceneComponent::Tick(double DeltaSeconds)
{
	if (bTickEnabled)
	{
		pAnimationList->Tick(DeltaSeconds);
		for (SceneComponent* Child : Children)
		{
			Child->Tick(DeltaSeconds);
		}
	}
}

void SceneComponent::AddAnimation(Animation* pAnimation_In)
{
	pAnimationList->Add(pAnimation_In);
}

void SceneComponent::SelectAnimation(int SequenceIndex)
{
	pAnimationList->ResetAnimation(SequenceIndex);
	pAnimationList->SetSequenceIndex(SequenceIndex);
}

int SceneComponent::RegisterInput_Float(float &Attribute)
{
	const int InputIndex = static_cast<int>(InputMap.size());
	Input *NewInput = new Input();
	NewInput->Type = EValueType::VALUE_FLOAT;
	NewInput->ValueFloat = &Attribute;
	InputMap.insert({ InputIndex, NewInput });
	return InputIndex;
}

int SceneComponent::RegisterInput_Vec3(Vector3f &Attribute)
{
	const int InputIndex = static_cast<int>(InputMap.size());
	Input *NewInput = new Input();
	NewInput->Type = EValueType::VALUE_VEC3;
	NewInput->ValueVec3 = &Attribute;
	InputMap.insert({ InputIndex, NewInput });
	return InputIndex;
}

int SceneComponent::RegisterInput_Bool(bool &Attribute)
{
	const int InputIndex = static_cast<int>(InputMap.size());
	Input *NewInput = new Input();
	NewInput->Type = EValueType::VALUE_BOOL;
	NewInput->ValueBool = &Attribute;
	InputMap.insert({ InputIndex, NewInput });
	return InputIndex;
}

void SceneComponent::UpdateInputs(Output* InOutput)
{
	for (OutputLink *Link : InOutput->Links)
	{
		Input *CurrentInput = InputMap[Link->InputIndex];
		switch (CurrentInput->Type)
		{
		case EValueType::VALUE_FLOAT:
			if (Link->OpType == EOperationType::OVERRIDE)
				*CurrentInput->ValueFloat = *InOutput->ValueFloat;
			else if (Link->OpType == EOperationType::ACCUMULATE)
				*CurrentInput->ValueFloat += *InOutput->ValueFloat;
			break;
		
		case EValueType::VALUE_VEC3:
			if (Link->OpType == EOperationType::OVERRIDE)
				*CurrentInput->ValueVec3 = *InOutput->ValueVec3;
			else if (Link->OpType == EOperationType::ACCUMULATE)
				*CurrentInput->ValueVec3 += *InOutput->ValueVec3;
			break;

		case EValueType::VALUE_BOOL:
			if (Link->OpType == EOperationType::OVERRIDE)
				*CurrentInput->ValueBool = *InOutput->ValueBool;
			else if (Link->OpType == EOperationType::ACCUMULATE) //Bool accumulate is a toggle.
				*CurrentInput->ValueBool = *CurrentInput->ValueBool == *InOutput->ValueBool ? !*CurrentInput->ValueBool : *InOutput->ValueBool;
			break;
		}
		ProcessInputEffects(&CurrentInput->InputEffects);
	}
}

// Overriden in Matter subclass that uses this to mark mesh data dirty.
void SceneComponent::ProcessInputEffects(TInputEffects const *InputEffects)
{
	bDirtyComponentParameters = InputEffects->bComponentParameters ? true : bDirtyComponentParameters;
}

void SceneComponent::Build()
{
	bDirtyComponentParameters = false;
}

bool SceneComponent::IsDirtyComponentParameters()
{
	return bDirtyComponentParameters;
}

vector<SceneComponent*>* SceneComponent::GetPrims()
{
	return &AllPrims;
}

vector<GLuint>* SceneComponent::GetOcclusionQueries()
{
	return &OcclusionQueries;
}