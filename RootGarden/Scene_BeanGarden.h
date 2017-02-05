#pragma once

#include "Scene.h"

class Scene_BeanGarden : public Scene
{
public:
	Scene_BeanGarden();
	~Scene_BeanGarden();

	void HandleInput(int Key, int x, int y, bool down) override;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool EscDown = false;
};

