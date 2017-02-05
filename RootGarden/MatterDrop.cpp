#include "MatterDrop.h"
#include "MeshRectangle_Instanced.h"

MatterDrop::MatterDrop()
	:
	Matter("MatterDrop", Counter<MatterDrop>::Count())
{
	Matter const &Ref = *this;
	MeshRectangle_Instanced* Rectangle= new MeshRectangle_Instanced(Ref);
	/*
	Rectangle->SetNumberOfSides(12);
	Rectangle->SetNumberOfLayers(4);
	Vector4f Color_A;
	Color_A << 0.0f, 0.0f, 0.0f, 0.4f;
	Vector4f Color_B;
	Color_B << 0.6f, 0.67f, 1.0f, 1.0f;
	Vector4f Color_C;
	Color_C << 0.6f, 0.67f, 1.0f, 1.0f;
	Vector4f Color_D;
	Color_D << 0.6f, 0.67f, 1.0f, 0.0f;

	Rectangle->SetLayerColor(0, Color_A);
	Rectangle->SetLayerColor(1, Color_B);
	Rectangle->SetLayerColor(2, Color_C);
	Rectangle->SetLayerColor(3, Color_D);

	Rectangle->SetLayerPosition(1, 0.6f);
	Rectangle->SetLayerPosition(2, 0.7f);
	*/
	SetScale(Vector3f::Constant(0.02f));
	Vector4f col;
	col << 0.3f, 1.0f, 0.4f, 0.5f;
	Rectangle->SetColor(col);
	ShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_instanced_prog");

	Mesh = Rectangle;
}

MatterDrop::~MatterDrop()
{
	delete Mesh;
}