#pragma once

#include "Matter.h"

class MatterSolidBeveledCube : public Counter<MatterSolidBeveledCube>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterSolidBeveledCube();
	~MatterSolidBeveledCube() override;
};