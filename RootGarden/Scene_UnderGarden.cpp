#include "Scene_UnderGarden.h"

Scene_UnderGarden::Scene_UnderGarden()
{
	MatterCircle* MatterCircleObject = new MatterCircle();
	MatterCircleObject->SetScale(Vector3f::Constant(0.5));
	/*
	MatterBean* MatterBeanObject = new MatterBean();
	
	//Vector4f BeanColor;
	//BeanColor << 0.95f, 0.875f, 0.775f, 1.0f;
	//MatterBeanObject->SetColors(BeanColor, BeanColor);
	
	Vector3f BeanScale;
	BeanScale << 0.01f, 0.01f, 0.01f;
	MatterBeanObject->Scale = BeanScale;
	
	AddChild(MatterBeanObject);
	*/
	AddChild(MatterCircleObject);

	MatterCircleObject = new MatterCircle();
	MatterCircleObject->SetScale(Vector3f::Constant(0.01f));
	Vector3f NewPosition;
	//NewPosition << -0.5f, -0.5f, 0.0f;
	Vector4f ColorInner;
	ColorInner << 1.0f, 1.0f, 1.0f, 1.0f;
	Vector4f ColorOuter;
	ColorOuter << 1.0f, 1.0f, 1.0f, 1.0f;

	MatterCircleObject->SetColors(ColorInner, ColorOuter);
	AddChild(MatterCircleObject);
}


Scene_UnderGarden::~Scene_UnderGarden()
{
}
