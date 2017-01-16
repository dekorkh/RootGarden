#pragma once

#include "SceneComponent.h"
#include "MatterMeter.h"
#include "MatterRectangle.h"
#include <deque>

class MatterMeterHead : public Matter
{
public:
	MatterMeterHead();
	~MatterMeterHead() override;

	/* Begin SceneComponent Interface*/
	void Build() override;
	/* End SceneComponent Interface*/

	/* Begin MatterInterface */
	void SetScale(const Vector3f& NewScale) override;
	void SetPosition(const Vector3f& NewHeadPosition);
	/* End MatterInterface */

	/* Parameters which can be registered as inputs.
	HeadPosition	-	The HeadPosition of the head on the meter, 0 is bottom, 1 is topped.
	HeadThickness	-	The HeadThickness of the head geometry as a fraction of the meter's height.
	*/
	float HeadPosition;
	float HeadThickness;

private:
	float HeadPosY0;
	float HeadPosY1;
	Vector3f LastPosition;
	Vector3f LastScale;
};