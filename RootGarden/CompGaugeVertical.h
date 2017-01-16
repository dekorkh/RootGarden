#pragma once

#include "SceneComponent.h"
#include <deque>

class MatterMeter;
class MatterMeterHead;

class CompGaugeVertical : public SceneComponent
{
public:
	CompGaugeVertical();

	/* Transform is meaningless to SceneComponent by default since 
	/ it doesn't necesserally contain a matter/mesh component.
	/ Each SceneComponent can decide to implement its own transforms
	/ specific to its design.
	*/
	void SetScale(const Vector3f& NewScale);
	void SetPosition(const Vector3f& NewPosition);

private:
	double ElapsedSeconds;
	
	MatterMeter* pMeter;
	MatterMeterHead* pHead;

	float Value;
	deque<float> ValueHistory;
	int NumHistory;
};