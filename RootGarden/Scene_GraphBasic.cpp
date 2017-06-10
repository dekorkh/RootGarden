#include "Scene_GraphBasic.h"
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

Scene_GraphBasic::Scene_GraphBasic() :
	InputType(1)
{
	// BACKGROUND PLANE
	rect = new MatterRectangle();
	Vector4f Teal;
	Teal << 0.65f, 0.85f, 0.85f, 1.0f;
	rect->SetColor(Teal);
	Vector3f Scale;
	Scale << 1.6f, 1.0f, 1.0f;
	rect->SetScale(Scale);
	Vector3f Position;
	Position << 0.0f, 0.0f, -1.0f;
	rect->SetPosition(Position);
	AddChild(rect);

	// PLOT
	CompGraph *Graph = new CompGraph();
	AddChild(Graph);
}


Scene_GraphBasic::~Scene_GraphBasic()
{
}

void Scene_GraphBasic::Build()
{
}

void Scene_GraphBasic::HandleInput(int Key, int x, int y, bool down)
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
	case 13: // Enter
		break;
	default:
		break;
	}
	Scene::HandleInput(Key, x, y, down);
}