#include "MatterBasicText.h"
#include "MeshRectangle.h"

MatterBasicText::MatterBasicText() :
	Matter("MatterBasicText", Counter<MatterBasicText>::Count())
{
	MeshRectangle* Rectangle = new MeshRectangle();
	Rectangle->SetColor(Vector4f(0.1f, 0.3f, 0.8f, 1.0f));
	Mesh = Rectangle;
}

MatterBasicText::~MatterBasicText()
{
	delete Mesh;
}