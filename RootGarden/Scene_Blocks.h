#pragma once

#include "Scene.h"

class CompSelector;
class CompWater;
class MatterRootTendril;

class Scene_Blocks : public Scene
{
public:
	Scene_Blocks();
	~Scene_Blocks();

	void HandleInput(int Key, int x, int y, bool down) override;

	void Build() override;

	GLfloat FrameTime;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool UKeyDown = false;
	bool DKeyDown = false;
	bool EscDown = false;

	MatterRectangle *Background;
	vector<vector<vector<MatterSolidBeveledCube*>>> Voxels;
	GLfloat const VoxelScale;
	GLint const VoxelDivisions;
};

