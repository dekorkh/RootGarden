#pragma once

#include "SceneComponent.h"
#include "MatterRootSegment.h"

class MatterRootTendril : public SceneComponent
{
public:
	MatterRootTendril();
	
	void Tick(double DeltaSeconds) override;

private:
	vector<Vector2f> Targets;

	void SpawnRoot(const MatterRootSegment* SourceSegment, const Vector3f& Target);

	int NumSegments;
	const int MaxSegments;

	bool bGrown;
};