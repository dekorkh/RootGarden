#include "Matter.h"
#include "GameStats.h"

Matter::Matter(const string& InName, const int InSerial) : SceneComponent(InName, InSerial)
{
	Initialize();
}

Matter::~Matter()
{
}

void Matter::Initialize()
{
	ShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");
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

void Matter::SetUniforms() const
{
	return Transform.SetUniform(ShaderProgram);
}

void Matter::Draw() const
{	
	SetUniforms();

	clock_t Clock_Draw = clock();
	Mesh->Draw(*ShaderProgram);
	GameStats::GetGameStats()->pThisFrame->Ms_Draw->Increment(TICKS_TO_MS(clock() - Clock_Draw));
}

void Matter::UpdateMesh(GenerateMeshResult *OutGenerateMeshResult)
{
	Transform.ComputeModelMatrix();
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