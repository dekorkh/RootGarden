#include "Scene_Texcube.h"
#include "Animation_Orbit.h"
#include "Animation_AimOrbit.h"
#include "Animation_Aim.h"
#include "Animation_Blank.h"
#include "Animation_Grow.h"
#include "Animation_Pulse.h"
#include "Animation_FrameTime.h"
#include "QTree.h"

#define _USE_MATH_DEFINES
#include "math.h"

Scene_Texcube::Scene_Texcube()
{
	MatterCircle* Rectangle = new MatterCircle();
	AddChild(Rectangle);
}


Scene_Texcube::~Scene_Texcube()
{
}

void Scene_Texcube::Build()
{

}

void Scene_Texcube::HandleInput(int Key, int x, int y, bool down)
{
	cout << "key: " << Key << "\n";
	switch (Key)
	{
	case 101:	//up
		if (down)
		{
			if (!UKeyDown)
			{
				UKeyDown = true;
			}
		}
		else
		{
			if (UKeyDown)
			{
				UKeyDown = false;
			}
		}
		break;
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
	case 103:	//down
		if (down)
		{
			if (!DKeyDown)
			{
				DKeyDown = true;
			}
		}
		else
		{
			if (DKeyDown)
			{
				DKeyDown = false;
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
	case 27:
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