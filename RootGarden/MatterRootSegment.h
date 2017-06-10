#pragma once

#include "Matter.h"

enum EGROWMODE
{
	TAPERED_TOWARD_EXPAND = 0,
	TAPERED_TOWARD_GROWOUT,
	POINTED_TOWARD_EXPAND,
	POINTED_TOWARD_GROWOUT,
	POINTED_INPLACE,
};

class MatterRootSegment : public Counter<MatterRootSegment>, public Matter
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

	MatterRootSegment();
	~MatterRootSegment() override;

	/* Begin Matter Interface */
	void Build() override;
	/* End Matter Interface */

	// Return the mass cost to reach target (assuming segment is oriented towards target)
	// Return 0.0f if distance to target is less than current length
	float MassCostToTarget(const Vector3f& Target) const;

	// Return the mass cost for adding a specified length
	float MassCostForLength(float InLength) const;

	// Return the position of the tip.
	Vector3f GetTipPosition() const;

	// Set a root segment to begin from
	void SetSourceRootSegment(const MatterRootSegment& SourceRootSegment);

	// Is the root grown
	bool bGrown;

	// Mass capacity of the root
	float MassCapacity;

	// LookAt
	Vector3f Param_Target;

private:
	//const
	float Density;
	float Taper;
	
	//var
	float InRadius;
	float MassContent;
	
	//derived
	float _Length;
	float _OutRadius;
	float _Volume;

	EGROWMODE GrowMode;
};

