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
#include "Scene_GraphBasic.h"
#include "Scene_Blocks.h"
#include "Scene_ColorCircles.h"
#include "Scene_UnderGarden.h"
#include "Scene_PrimeGarden.h"
#include "Scene_TimeGarden.h"
#include "Scene_RootGarden.h"
#include "Scene_BeanGarden.h"
#include "Scene_Fabric.h"
#include "Scene_Texcube.h"
#include "GlobalState.h"
#include "GameStats.h"
#include "ShaderManager.h"

using namespace std;
using namespace Eigen;

class Camera;

enum EViewMode
{
	E_MODE_FINAL = 0,
	E_MODE_OCCLUSION_DEPTH
};

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

	double DeltaSeconds_Display;
	clock_t ClockLastFrame;
	clock_t ClockMinFrame;
	clock_t ClockFrameLimiter;

	int FrameIndex;

private:
	PFNWGLSWAPINTERVALEXTPROC glSwapInterval = NULL;
	PFNWGLGETSWAPINTERVALEXTPROC glGetSwapInterval = NULL;

	/* SetupPrimitives
	/ Traverse the primitives and update any dirty aspects s.a. async compute their modelmatrix, generate any meshes as needed, etc.
	*/
	void SetupPrimitives(vector<SceneComponent*> const &InPrims) const;

	/* SetupCamera
	/ Update any dirty data on the camera s.a. its model matrix.
	*/
	void SetupCamera(Camera &InCamera) const;

	// Current aspect ratio
	GLfloat AspectRatio;

	// ViewMode - allows for previewing different buffers and such.
	EViewMode ViewMode;
};