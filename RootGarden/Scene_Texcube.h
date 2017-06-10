#pragma once

#include "Scene.h"

class CompSelector;
class CompWater;

class Scene_Texcube : public Scene
{
public:
	Scene_Texcube();
	~Scene_Texcube();

	void HandleInput(int Key, int x, int y, bool down) override;

	void Build() override;

	GLfloat FrameTime;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool UKeyDown = false;
	bool DKeyDown = false;
	bool EscDown = false;
};

#pragma once
