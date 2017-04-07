#pragma once

#include "Scene.h"

class CompSelector;
class CompWater;

class Scene_BeanGarden : public Scene
{
public:
	Scene_BeanGarden();
	~Scene_BeanGarden();

	void HandleInput(int Key, int x, int y, bool down) override;

	void Build() override;

	GLfloat FrameTime;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool UKeyDown = false;
	bool DKeyDown = false;
	bool EscDown = false;

	CompSelector *Selector;
	MatterCircle* pRoot;
	CompWater* pWater;

	Vector3f IncrementVelocityUp;
	Vector3f IncrementVelocityDown;
	Vector3f IncrementVelocityLeft;
	Vector3f IncrementVelocityRight;
};

