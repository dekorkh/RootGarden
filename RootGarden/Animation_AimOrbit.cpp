#include "Animation_AimOrbit.h"
#include "Matter.h"
#include "Eigen/Dense"

using namespace Eigen;

Animation_AimOrbit::Animation_AimOrbit() : OrbitSpeed(1.0f), Out(Vector3f::Constant(0.0f))
{
}

Animation_AimOrbit::~Animation_AimOrbit()
{
}

bool Animation_AimOrbit::Tick(const double DeltaSeconds)
{
	Animation::Tick(DeltaSeconds);

	float Radians = static_cast<float>(ElapsedSeconds * M_PI);
	float posX = sin(Radians * OrbitSpeed);
	float posY = cos(Radians * OrbitSpeed);
	Out(0) = posX;
	Out(1) = posY;
	Out(3) = 0.0f;
	
	return true;
}

void Animation_AimOrbit::SetOrbitSpeed(float OrbitSpeedIn)
{
	OrbitSpeed = OrbitSpeedIn;
}