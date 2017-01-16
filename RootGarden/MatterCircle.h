#pragma once

#include "Matter.h"
#include "MeshCircle.h"

class MatterCircle : public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	MatterCircle();
	~MatterCircle() override;

	void SetColors(const Vector4f& InColorInner, const Vector4f& InColorOuter);
	//virtual void Draw(GLint ProgramAddress) override;

	GLuint NumOfSides = 180;
	GLfloat Radius = 1.0f;

	Vector4f CenterColor;
	Vector4f OuterColor;
};