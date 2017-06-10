#pragma once

#include "SceneComponent.h"

class Animation_EaseInVec3;
class Animation_AccumulateVec3;
class MatterCircle;
class Matter;

class CompSelector : public Counter<CompSelector>, public SceneComponent
{
public:
	CompSelector();
	~CompSelector() override;

	void Build() override;

	void GoToPosition(Vector3f InPosition);
	void Select(Matter const &Target);
	void SelectLeft();
	void SelectRight();
	void SelectUp();
	void SelectDown();

	void AddVelocity(Vector3f const Velocity);
	void KillVelocity();

	MatterCircle *Outline;

	Vector3f Input_LookAt;
	Vector3f Input_Position;

	Animation_EaseInVec3 *GoToAnim;
	Animation_AccumulateVec3 *VelocityAnim;

	Matter const *Selected;
};
