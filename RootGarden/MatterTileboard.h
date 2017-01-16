#pragma once

#include "Matter.h"
#include "MatterRectangleGradient.h"

#include "Eigen\Dense"

class MatterTileboard : public SceneComponent
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	MatterTileboard();
	~MatterTileboard() override;

	GLfloat TileSide;

	vector<vector<MatterRectangleGradient*>> Tiles;

	float ActivePointX;
	float ActivePointY;

	void Build() override;

private:
	MatterRectangleGradient* MakeTile();
};