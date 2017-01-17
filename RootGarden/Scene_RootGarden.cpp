#include "Scene_RootGarden.h"
#include "Animation_Orbit.h"
#include "Animation_AimOrbit.h"
#include "Animation_Aim.h"
#include "Animation_Blank.h"
#include "Animation_Grow.h"
#include "Animation_Pulse.h"

#define _USE_MATH_DEFINES
#include "math.h"

Scene_RootGarden::Scene_RootGarden()
{
	MatterTileboard* tileBoard = new MatterTileboard();
	AddChild(tileBoard);
	
	MatterRectangle* Rectangle = new MatterRectangle();
	Rectangle->SetLookAtTarget(Vector3f(0.5f, 0.5f, 0.0f));
	Rectangle->SetPosition(Vector3f(-1.2f, -0.5f, 0.0f));
	Rectangle->SetScale(Vector3f(0.2f, 0.2f, 1.0f));
	AddChild(Rectangle);
	
	MatterRootTendril *tendril = new MatterRootTendril();
	AddChild(tendril);
	
	MatterRootCluster *cluster = new MatterRootCluster();
	AddChild(cluster);
	
	MatterRing *Ring = new MatterRing();
	AddChild(Ring);

	CompGaugeVertical *GaugeV = new CompGaugeVertical();
	GaugeV->SetScale(Vector3f(0.05f, 0.2f, 1.0f));
	GaugeV->SetPosition(Vector3f(-1.2f, 0.5f, 1.0f));
	AddChild(GaugeV);
	/*
	MatterBasicText* BasicText = new MatterBasicText();
	AddChild(BasicText);
	*/
}


Scene_RootGarden::~Scene_RootGarden()
{
}

void Scene_RootGarden::HandleInput(int Key, int x, int y, bool down)
{
	switch (Key)
	{
	case 102:	//right
		if (down)
		{
			if (!RKeyDown)
			{
				RKeyDown = true;
			}
		}
		else
		{
			if (RKeyDown)
			{
				RKeyDown = false;
			}
		}
		break;
	case 100: //left
		if (down)
		{
			if (!LKeyDown)
			{
				LKeyDown = true;
			}
		}
		else
		{
			if (LKeyDown)
			{
				LKeyDown = false;
			}
		}
		break;
	case 27: //left
		if (down && !EscDown)
		{
			EndScene();
			EscDown = true;
		}
		else if (EscDown)
		{
			EscDown = false;
		}
		break;
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}