#include "Animation_Orbit.h"
#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_Orbit::Animation_Orbit()
{
}

Animation_Orbit::~Animation_Orbit()
{
}

bool Animation_Orbit::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	float posX = static_cast<float>(sin(ElapsedSeconds * M_PI));
	float posY = static_cast<float>(cos(ElapsedSeconds * M_PI));
	Out(0) = posX;
	Out(1) = posY;
	Out(2) = 0.0f;

	return true;
}