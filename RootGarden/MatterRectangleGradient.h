#pragma once

#include "Matter.h"

class MatterRectangleGradient : public Matter
{
public:
	MatterRectangleGradient();

	void Build() override;

	void Pulse(bool bForce = true);
	void Draw() const override;

	float Glow;
	int InputIdx_Glow;	

	bool bPulsing;
	int InputIdx_bPulsing;
};