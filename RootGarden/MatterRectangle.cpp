#include "MatterRectangle.h"
#include "MeshRectangle.h"

MatterRectangle::MatterRectangle() :
	Matter("MatterRectangle", Counter<MatterRectangle>::Count())
{
	Rect = new MeshRectangle();
	Vector4f RectangleColor;
	RectangleColor << 1.0f, 1.0f, 1.0f, 1.0f;
	Rect->SetColor(RectangleColor);
	Mesh = Rect;
	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("textureColor_prog");
	pShaderProgram->AddUniform("sampler_Color", UNIFORM_I1);
	pShaderProgram->SetUniform("sampler_Color", &TextureUnit_Color);
}

MatterRectangle::~MatterRectangle()
{
	delete Mesh;
}

void MatterRectangle::SetColor(Vector4f const &InColor)
{
	Rect->SetColor(InColor);
}