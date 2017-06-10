#pragma once

#include "Matter.h"

class MeshRectangle;

class MatterLine : public Counter<MatterLine>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterLine();
	~MatterLine() override;

	void SetColor(Vector4f const &InColor);
	void SetPositions(vector<GLfloat> &InPositions);
};