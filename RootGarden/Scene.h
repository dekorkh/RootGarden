#pragma once

#include "Eigen/StdVector"
#include "SceneComponent.h"
#include "MatterTypes.h"
#include "GameStats.h"
#include "Scene.h"
#include "GlobalState.h"
#include "Camera.h"

class Scene : public SceneComponent
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Scene();

	void HandleInput(int Key, int x, int y, bool down) override;
	void EndScene();

	void Tick(double deltaSeconds) override;

	Vector4f BackgroundColor;
	double ElapsedSeconds;

	Camera *ActiveCamera;
};

