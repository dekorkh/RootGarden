#pragma once

#include "Scene.h"

class Scene_RootGarden : public Scene
{
public:
	Scene_RootGarden();
	~Scene_RootGarden();

	void HandleInput(int Key, int x, int y, bool down) override;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool EscDown = false;
};

