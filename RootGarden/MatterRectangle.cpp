#include "MatterRectangle.h"
#include "MeshRectangle.h"

MatterRectangle::MatterRectangle() :
	Matter("MatterRectangle", Counter<MatterRectangle>::Count())
{
	MeshRectangle* Rectangle = new MeshRectangle();
	Vector4f RectangleColor;
	RectangleColor << 0.0f, 1.0f, 0.0f, 1.0f;
	Rectangle->SetColor(RectangleColor);
	Mesh = Rectangle;
}