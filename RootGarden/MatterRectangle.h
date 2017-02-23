#pragma once

#include "Matter.h"

class MeshRectangle;

class MatterRectangle : public Counter<MatterRectangle>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterRectangle();
	~MatterRectangle() override;

	void SetColor(Vector4f const &InColor);

	MeshRectangle* Rect;
};