#pragma once
#include "Matter.h"

class MatterRectangle_Instanced :
	public Counter<MatterRectangle_Instanced>,
	public Matter
{
public:
	MatterRectangle_Instanced();
	virtual ~MatterRectangle_Instanced();
};

