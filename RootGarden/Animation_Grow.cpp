#include "Animation_Grow.h"

#include "Matter.h"
#include "Eigen/Dense"
#include "MatterRootSegment.h"

using namespace Eigen;

Animation_Grow::Animation_Grow() : Amount(0.0f)
{
}

Animation_Grow::Animation_Grow(float InGrams) 
	: Amount(InGrams)
{
}

Animation_Grow::~Animation_Grow()
{
}

bool Animation_Grow::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	float GrowAmount = Rate * static_cast<float>(DeltaSeconds);
	GrowAmount = GrowAmount <= Amount ? GrowAmount : Amount;
	Amount -= GrowAmount;
	Amount = Amount > 0.0f ? Amount : 0.0f;

	Out = GrowAmount;
	return true;
}

void Animation_Grow::SetRate(float GramsPerSecond)
{
	Rate = GramsPerSecond;
}

bool Animation_Grow::Finished() const
{
	return Amount > 0.0f;
}