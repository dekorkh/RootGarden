#include "MatterRectangle.h"
#include "MeshRectangle.h"

MatterRectangle::MatterRectangle() :
	Matter("MatterRectangle", Counter<MatterRectangle>::Count())
{
	Rect = new MeshRectangle();
	Vector4f RectangleColor;
	RectangleColor << 0.0f, 1.0f, 0.0f, 1.0f;
	Rect->SetColor(RectangleColor);
	Mesh = Rect;
}

MatterRectangle::~MatterRectangle()
{
	delete Mesh;
}

void MatterRectangle::SetColor(Vector4f const &InColor)
{
	Rect->SetColor(InColor);
}