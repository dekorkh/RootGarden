#pragma once

#include "GL/glew.h"
#include <iostream>
#include <fstream>
#include "GL/freeglut.h"
#include "glext.h"
#include "wglext.h"
#include "Eigen/StdVector"
#include "Eigen/Dense"	//vectors
#include <time.h>
#include <future>
#include <thread>

#include "Scene.h"
#include "Scene_ColorCircles.h"
#include "Scene_UnderGarden.h"
#include "Scene_PrimeGarden.h"
#include "Scene_TimeGarden.h"
#include "Scene_RootGarden.h"
#include "GlobalState.h"
#include "GameStats.h"
#include "ShaderManager.h"

using namespace std;
using namespace Eigen;

class Game
{
public:
	Game();
	~Game();
	void HandleInput(int Key, int x, int y, bool down);
	void InitGraphics(Vector2i WindowSize);
	void Display();

	ofstream* Log_Frame;

	GameStats* pGameStats; // Keeps statistics for each frame.

	Scene* ActiveScene;

	GLfloat AspectRatio = 1.0;

	double DeltaSeconds_Display;
	clock_t ClockLastFrame;
	clock_t ClockMinFrame;
	clock_t ClockFrameLimiter;

	int FrameIndex;

private:
	PFNWGLSWAPINTERVALEXTPROC glSwapInterval = NULL;
	PFNWGLGETSWAPINTERVALEXTPROC glGetSwapInterval = NULL;

	/* GatherPrimitives
	/ Traverse the primitives and generate any meshes as needed.
	*/
	void GatherPrimitives(vector<SceneComponent*>* OutPrims);
	vector<SceneComponent*> Prims;
};