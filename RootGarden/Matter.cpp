#include "Matter.h"
#include "Mesh.h"
#include "GameStats.h"

Matter::Matter(const string& InName, const int InSerial) 
	: 
	SceneComponent(InName, InSerial),
	bIsStencil(false),
	bIsOcclusion(false)
{
	Initialize();
}

Matter::~Matter()
{
}

void Matter::Initialize()
{
	ShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");
	ShaderProgram->SetUniform("ModelMatrix", Transform.GetModelMatrixDataPtr());
}

void Matter::SetScale(const Vector3f& InScale)
{
	Transform.SetScale(InScale);
	bDirtyModelMatrix = true;
}

void Matter::SetPosition(const Vector3f& InPosition)
{
	Transform.SetPosition(InPosition);
	bDirtyModelMatrix = true;
}

void Matter::SetLookAt(const Vector3f& InLookAt)
{
	Transform.SetLookAt(InLookAt);
	bDirtyModelMatrix = true;
}

void Matter::SetLookAtTarget(const Vector3f& InTarget)
{
	Transform.SetLookAt((InTarget - Transform.GetPosition()).normalized());
	bDirtyModelMatrix = true;
}

void Matter::AddRotation(const float Deg)
{
	Transform.AddRotation(Deg);
	bDirtyModelMatrix = true;
}

void Matter::ProcessInputEffects(TInputEffects const *InputEffects)
{
	SceneComponent::ProcessInputEffects(InputEffects);
	Mesh->bDirty_Positions = InputEffects->bVertPositions ? true: Mesh->bDirty_Positions;
	Mesh->bDirty_Colors = InputEffects->bVertColors ? true : Mesh->bDirty_Colors;
	Mesh->bDirty_Indices = InputEffects->bVertIndices ? true : Mesh->bDirty_Indices;
}

void Matter::Draw()
{
	// so I'll re-get the pointer but it'd be nice to know why the pData is null in the uniform at this point.
	ShaderProgram->SetUniform("ModelMatrix", Transform.GetModelMatrixDataPtr());
	
	clock_t Clock_SetUniforms = clock();
	if (ShaderProgram->ProgAddr != ShaderProgram::LastProgAddr)
	{
		GameStats::GetGameStats()->pThisFrame->Ms_SetUniforms->Increment(TICKS_TO_MS(clock() - Clock_SetUniforms));
		//Set the new shader program
		ShaderProgram->Use();
		
		// Set global uniforms
		ShaderProgram->PushGlobalUniforms();

		// Push global uniforms only pushes the uniforms that were marked dirty.
		Clock_SetUniforms = clock();
	}
	
	ShaderProgram->PushUniforms();

	
	GameStats::GetGameStats()->pThisFrame->Ms_SetUniforms->Increment(TICKS_TO_MS(clock() - Clock_SetUniforms));
	
	clock_t Clock_Draw = clock();
	Mesh->Draw(*ShaderProgram);
	GameStats::GetGameStats()->pThisFrame->Ms_Draw->Increment(TICKS_TO_MS(clock() - Clock_Draw));
}

void Matter::UpdateMesh(GenerateMeshResult *OutGenerateMeshResult)
{
	Mesh->GenerateMesh(OutGenerateMeshResult);	
}

bool Matter::IsMeshDirty()
{
	return (Mesh->bDirty_Positions || Mesh->bDirty_Colors || Mesh->bDirty_Indices);
}

bool Matter::IsModelMatrixDirty()
{
	return bDirtyModelMatrix;
}

void Matter::UpdateModelMatrix()
{
	Transform.ComputeModelMatrix();
	bDirtyModelMatrix = false;
}