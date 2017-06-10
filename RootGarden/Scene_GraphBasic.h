#pragma once

#include "Scene.h"

class CompSelector;
class CompWater;
class MatterRootTendril;

class Scene_GraphBasic : public Scene
{
public:
	Scene_GraphBasic();
	~Scene_GraphBasic();

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
	MatterRootTendril *LastRootTendril;
	MatterRootSegment *LastRootSegment;
	MatterRectangle *rect;

	Vector3f IncrementVelocityUp;
	Vector3f IncrementVelocityDown;
	Vector3f IncrementVelocityLeft;
	Vector3f IncrementVelocityRight;

	// 0 is free, 1 is snap to nearest.
	int InputType;
};

