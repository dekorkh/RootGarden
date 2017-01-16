#pragma once

#include "Eigen/StdVector"
#include "MatterTypes.h"
#include "SceneComponent.h"
#include "GameStats.h"

class Scene : public SceneComponent
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	Scene();

	void HandleInput(int Key, int x, int y, bool down) override;
	void EndScene();

	Vector4f BackgroundColor;
	double ElapsedSeconds;
};

