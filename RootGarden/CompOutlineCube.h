#pragma once

#include "SceneComponent.h"

class Animation_LerpVec3;
class MatterWireCube;
class MatterSolidCube;

class CompOutlineCube : public Counter<CompOutlineCube>, public SceneComponent
{
public:
	CompOutlineCube();
	~CompOutlineCube() override;

	void Build() override;

	MatterWireCube *WireCube;
	MatterSolidCube *SolidCube;

	Vector3f Input_LookAt;
};
