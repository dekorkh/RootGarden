#pragma once

#include "Matter.h"


class MatterMeter : public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	MatterMeter();
	~MatterMeter() override;
};
