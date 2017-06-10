#pragma once

#include "Matter.h"

class MeshRectangle;

class MatterPlot : public Counter<MatterPlot>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterPlot();
	~MatterPlot() override;

	void SetColor(Vector4f const &InColor);
	void SetCenter(Vector3f const &InCenter);
	void SetUnitScale(Vector3f const &InUnitScale);

private:

	GLfloat UnitScale;
};