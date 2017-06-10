#pragma once

#include "TransformInfo.h"

class Camera
{
public:
	Camera();
	virtual ~Camera();

	TransformInfo Transform;

	void Tick(double DeltaSeconds);

	double ElapsedSeconds;
};