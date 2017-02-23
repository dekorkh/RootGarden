#pragma once

#include "SceneComponent.h"

class Animation_AccumulateVec3;
class MatterWireCube;
class MatterSolidCube;

class CompTravelCube : public Counter<CompTravelCube>, public SceneComponent
{
public:
	CompTravelCube();
	~CompTravelCube() override;

	void Build() override;

	void AddVelocity(Vector3f Velocity);

	MatterWireCube *WireCube;
	MatterSolidCube *SolidCube;

	Vector3f Input_LookAt;
	Vector3f Input_Position;

	Animation_AccumulateVec3 *VelocityAnim;
};
