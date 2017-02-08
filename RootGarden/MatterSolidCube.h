#pragma once

#include "Matter.h"

class MatterSolidCube : public Counter<MatterSolidCube>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterSolidCube();
	~MatterSolidCube() override;
};