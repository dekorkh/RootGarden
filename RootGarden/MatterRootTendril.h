#pragma once

#include "SceneComponent.h"
#include "MatterRootSegment.h"

class MatterRootTendril : public SceneComponent
{
public:
	MatterRootTendril();
	
	void Tick(double DeltaSeconds) override;
	
	MatterRootSegment* SpawnRoot(const MatterRootSegment* SourceSegment, const Vector3f& Target);

private:
	vector<Vector2f> Targets;

	int NumSegments;
	const int MaxSegments;

	bool bGrown;
};