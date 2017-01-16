#pragma once

#include "Scene.h"

class Scene_TimeGarden : public Scene
{
public:
	Scene_TimeGarden();
	~Scene_TimeGarden();

	void Tick(double DeltaSeconds) override;
	void HandleInput(int Key, int x, int y, bool down) override;


	MatterCircle* Circle;
	MatterRootSegment* pMatterRootSegment;
	bool TurnComplete;

	bool RKeyDown = false;
	bool LKeyDown = false;
	bool EscDown = false;
};

