#pragma once

#include "Animation.h"

class Animation_Blank : public Animation
{
public:
	Animation_Blank();
	~Animation_Blank();

	bool Tick(const double DeltaSeconds) override;
};

