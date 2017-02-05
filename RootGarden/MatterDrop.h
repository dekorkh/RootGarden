#pragma once

#include "Matter.h"

class MatterDrop : public Counter<MatterDrop>, public Matter
{
public:
	MatterDrop();
	~MatterDrop() override;
};