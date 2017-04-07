#include "MatterRectangleGradient.h"
#include "MeshRectangleGradient.h"
#include "Animation_Pulse.h"
#include "Animation_Blank.h"
#include "Animation_Trigger.h"

MatterRectangleGradient::MatterRectangleGradient() :
	Matter("MatterRectangleGradient", Counter<MatterRectangleGradient>::Count())
{	
	Mesh = new MeshRectangleGradient();
	InputIdx_Glow = RegisterInput_Float(Glow);
	InputIdx_bPulsing = RegisterInput_Bool(bPulsing);
	
	Animation_Pulse *AnimPulse = new Animation_Pulse(1.0f);
	Animation_Blank *AnimBlank = new Animation_Blank();
	Animation_Trigger *AnimTrigger = new Animation_Trigger(false);
	
	AnimPulse->LinkOutput(AnimPulse->OutputIdx_Out, InputIdx_Glow, EOperationType::OVERRIDE);
	AnimTrigger->LinkOutput(AnimTrigger->OutputIdx_Out, InputIdx_bPulsing, EOperationType::ACCUMULATE);
	
	AddAnimation(AnimTrigger);
	AddAnimation(AnimPulse);
	AddAnimation(AnimTrigger);
	AddAnimation(AnimBlank);
	
	SelectAnimation(3);

	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("param_alpha_prog");
	pShaderProgram->SetUniform("ParamAlpha", &Glow);

}

void MatterRectangleGradient::Build()
{
	SceneComponent::Build();

	MeshRectangleGradient *Rectangle = static_cast<MeshRectangleGradient*>(Mesh);
	
	Rectangle->SetNumberOfLayers(4);

	Vector4f ColorA, ColorB, ColorC, ColorD;

	ColorA << 1.0f, 1.0f, 1.0f, 8.0f;
	ColorB << 1.0f, 0.8f, 0.8f, 8.0f;
	ColorC << 0.8f, 0.0f, 0.5f, 1.0f;
	ColorD << 0.8f, 0.0f, 0.0f, 0.0f;

	Rectangle->SetLayerColor(0, ColorA);
	Rectangle->SetLayerColor(1, ColorB);
	Rectangle->SetLayerColor(2, ColorC);
	Rectangle->SetLayerColor(3, ColorD);

	Rectangle->SetLayerPosition(1, 0.3f);
	Rectangle->SetLayerPosition(2, 0.31f);
}

void MatterRectangleGradient::Pulse(bool bForce)
{
	if (bForce || !bPulsing)
		SelectAnimation(0);
}