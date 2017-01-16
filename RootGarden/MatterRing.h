#pragma once

#include "Matter.h"


class MatterRing : public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	MatterRing();
	~MatterRing() override;

	GLuint NumOfSides = 180;
	GLfloat Radius = 1.0f;
};