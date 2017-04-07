#include "MatterDrop.h"
#include "MeshRectangle_Instanced.h"
#include "MeshCircle.h"
#include "math.h"

using namespace std;

MatterDrop::MatterDrop()
	:
	Matter("MatterDrop", Counter<MatterDrop>::Count())
{
	MeshCircle* NewMesh = new MeshCircle();
	
	Vector4f NewColorInner;
	NewColorInner << 0.0f, 0.0f, 0.0f, 0.2f;
	Vector4f NewColorOuter;
	NewColorOuter << 0.7f, 0.8f, 1.0f, 1.0f;
	NewMesh->SetColors(NewColorInner, NewColorOuter);
	
	NewMesh->SetNumSides(20);
	SetScale(Vector3f::Constant(0.01f));
	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");

	Mesh = NewMesh;
}

MatterDrop::~MatterDrop()
{
	delete Mesh;
}

void MatterDrop::SetColor(Vector4f InColor)
{
	MeshCircle* circle = dynamic_cast<MeshCircle*>(Mesh);
	Vector4f NewColorInner;
	NewColorInner << 0.0f, 0.0f, 0.0f, 0.2f;
	circle->SetColors(NewColorInner, InColor);
	circle->bDirty_Colors = true;
}