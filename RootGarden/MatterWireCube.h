#pragma once

#include "Matter.h"

class MatterWireCube : public Counter<MatterWireCube>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterWireCube();
	~MatterWireCube() override;
};