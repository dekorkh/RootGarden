#pragma once

#include "Matter.h"

class MatterRectangle : public Counter<MatterRectangle>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
	MatterRectangle();
	~MatterRectangle() override;
};