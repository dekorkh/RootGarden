#include "Matter.h"
#include "Mesh.h"
#include "GameStats.h"
#include "Scene.h"

Matter::Matter(const string& InName, const int InSerial) 
	: 
	SceneComponent(InName, InSerial),
	bIsStencil(false),
	bIsOcclusion(false),
	TextureUnit_Color(ETEXTURE_COLOR),
	bDirtyModelMatrix(true)
{
	Initialize();
}

Matter::~Matter()
{
}

void Matter::Initialize()
{
	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");
	pShaderProgram->SetUniform("ModelMatrix", Transform.GetModelMatrixDataPtr());
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

void Matter::Draw(Scene const &InScene)
{
	// so I'll re-get the pointer but it'd be nice to know why the pData is null in the uniform at this point.
	pShaderProgram->SetUniform("ModelMatrix", Transform.GetModelMatrixDataPtr());

	clock_t Clock_SetUniforms = clock();
	if (pShaderProgram->ProgAddr != ShaderProgram::LastProgAddr)	// Runs at least once per frame per shader prog. since LastProgAddr is set to null at end of frame.
	{
		GameStats::GetGameStats()->pThisFrame->Ms_SetUniforms->Increment(TICKS_TO_MS(clock() - Clock_SetUniforms));
		//Set the new shader program
		pShaderProgram->Use();
		
		// Update the camera uniform
		pShaderProgram->SetGlobalUniform("ViewMatrix", InScene.ActiveCamera->Transform.GetModelMatrixDataPtr());
		pShaderProgram->MarkGlobalUniformDirty("ViewMatrix", true);

		// Set global uniforms
		pShaderProgram->PushGlobalUniforms();		

		// Push global uniforms only pushes the uniforms that were marked dirty.
		Clock_SetUniforms = clock();
	}
	
	pShaderProgram->PushUniforms();

	
	GameStats::GetGameStats()->pThisFrame->Ms_SetUniforms->Increment(TICKS_TO_MS(clock() - Clock_SetUniforms));
	
	clock_t Clock_Draw = clock();
	Mesh->Draw(*pShaderProgram);
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