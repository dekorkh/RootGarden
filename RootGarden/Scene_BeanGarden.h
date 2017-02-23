#pragma once

#include "Scene.h"

class CompSelector;

class Scene_BeanGarden : public Scene
{
public:
	Scene_BeanGarden();
	~Scene_BeanGarden();

	void HandleInput(int Key, int x, int y, bool down) override;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool UKeyDown = false;
	bool DKeyDown = false;
	bool EscDown = false;

	CompSelector *Selector;
	MatterCircle* pRoot;

	Vector3f IncrementVelocityUp;
	Vector3f IncrementVelocityDown;
	Vector3f IncrementVelocityLeft;
	Vector3f IncrementVelocityRight;
};

