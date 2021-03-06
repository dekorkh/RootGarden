#pragma once

#include "Matter.h"

class MatterRectangleGradient : public Matter
{
public:
	MatterRectangleGradient();

	/* Begin SceneComponent Interface */
	void Build() override;
	/* End SceneComponent Interface */

	void Pulse(bool bForce = true);
	

	float Glow;
	int InputIdx_Glow;	

	bool bPulsing;
	int InputIdx_bPulsing;
};