#include "MatterRootSegment.h"
#include "MatterRectangle.h"
#include "MeshTaperedCylinder.h"
#define Pi static_cast<float>(M_PI)

MatterRootSegment::MatterRootSegment() : 
	Matter("MatterRootSegment", Counter<MatterRootSegment>::Count())
{

	GrowMode = EGROWMODE::POINTED_TOWARD_GROWOUT;

	Density = 3.0f;
	Taper = 0.7f;

	//var
	InRadius = 0.05f;
	MassContent = 0.0f;
	MassCapacity = 0.0f;

	//derived
	_Length = 0.0f;
	_OutRadius = 0.0f;
	_Volume = 0.0f;
	
	Mesh = new MeshTaperedCylinder();
	static_cast<MeshTaperedCylinder*>(Mesh)->SetBaseWidth(0.0f);
	static_cast<MeshTaperedCylinder*>(Mesh)->SetTipWidth(0.0f);

	bGrown = false;

	Param_Target << 1.0f, 0.0f, 0.0f;
}


MatterRootSegment::~MatterRootSegment()
{
	delete Mesh;
}

void MatterRootSegment::Build() 
{
	Matter::Build();

	////////////////////////////////////////////
	//v = 1/3 L pi r^2
	//L = (v * 3) / (pi * r^2)
	////////////////////////////////////////////
	// Vt = M / D
	// Vt = Vb - Vs
	// Vs = Vb - Vt
	// Vb = Vt + Vs

	// Vb relates to Vs via lengths
	//	Vb = 1/3 * Lb * pi * Rb^2
	//	Vs = 1/3 * Ls * pi * Rs^2

	//	Ls = Lb * T
	//	Rs = Rb * T

	//	Vs = 1/3 * Lb * T * pi * (Rb * T)^2		--- Vs in terms of Vb

	// Vt, Rb, T are knowns and Lb is not... solve for Lb in those terms

	// Solve for Lb
	// Vt = (1 / 3 * Lb * pi * Rb ^ 2) - (1 / 3 * Lb * T * pi * (Rb * T) ^ 2)
	// Vt / Lb = (1 / 3 * pi * Rb ^ 2) - (1 / 3 * T * pi * (Rb * T) ^ 2)
	// Lb = Vt / ((1 / 3 * pi * Rb ^ 2) - (1 / 3 * T * pi * (Rb * T) ^ 2))
	
	_Volume = MassCapacity / Density;

	switch (GrowMode)
	{
		case EGROWMODE::TAPERED_TOWARD_EXPAND:
		{
			_OutRadius = InRadius * Taper;
			float termA = (1.0f / 3.0f) * Pi * pow(InRadius, 2.0f);
			float termB = (1.0f / 3.0f) * Taper * Pi * pow(InRadius * Taper, 2);
			float Lb = _Volume / (termA - termB);
			_Length = Lb * (1.0f - Taper);	// Length tapered cylinder
			break;
		}
		case EGROWMODE::POINTED_TOWARD_GROWOUT:
		{
			// Given a volume and base radius, compute a cone length
			_OutRadius = 0.0f;
			// Volume = Pi * r^2 * (Length / 3);
			// Length = (Volume / (Pi * r^2)) * 3;
			_Length = (_Volume / (Pi * pow(InRadius, 2.0f))) * 3.0f;
			break;
		}
		case EGROWMODE::POINTED_INPLACE:
		{
			// Given volume of a tapered cone, length, and radius of its larger base,
			// Imagine the tapered height was height of a cone - the ratio of its volume to the volume of the tapered cone is the ratio of 
			break;
		}
	}

	static_cast<MeshTaperedCylinder*>(Mesh)->SetBaseWidth(InRadius);
	static_cast<MeshTaperedCylinder*>(Mesh)->SetTipWidth(_OutRadius);
	static_cast<MeshTaperedCylinder*>(Mesh)->SetLength(_Length);

	SetLookAtTarget(Param_Target.normalized() + Transform.GetPosition());
}

float MatterRootSegment::MassCostForLength(float InLength) const
{
	float MassCost = 0.0f;
	switch (GrowMode)
	{
	case EGROWMODE::TAPERED_TOWARD_EXPAND:
	{
		//v = 1/3 L pi r^2
		//L = (v * 3) / (pi * r^2)

		float Rb_current = _Length > 0.0f ? InRadius * Taper : InRadius;

		// Current endpoint as percent of projected length
		float percentProjectedLength = _Length / (_Length + InLength);

		// Mass big cone
		float Rb = percentProjectedLength * (InRadius - _OutRadius) + InRadius;
		float Rs = InRadius * Taper;

		float Lb = Rb / ((Rb - Rs) / InLength);
		float Mb = 1.0f / 3.0f * static_cast<float>(M_PI) * Lb * pow(Rb, 2.0f);

		// Mass small cone

		float Ls = Lb - InLength;
		float Ms = 1.0f / 3.0f * static_cast<float>(M_PI) * Ls * pow(Rs, 2.0f);

		// Mass tapered cone (big cone - small cone)
		MassCost = Mb - Ms;
		break;
	}
	case EGROWMODE::POINTED_TOWARD_GROWOUT:
	{
		//Given a length, base radius, and density, compute amount of mass needed for a simple cone.
		// Volume = Pi * r^2 * (Length / 3);
		// Mass = Volume * Density;
		MassCost = Pi * pow(InRadius, 2.0f) * (InLength / 3) * Density;
		break;
	}
	}
	return MassCost;
}

float MatterRootSegment::MassCostToTarget(const Vector3f& Target) const
{
	float DistanceToTarget = (Target - GetTipPosition()).norm();
	if (DistanceToTarget > 0.0f)
	{
		return MassCostForLength(DistanceToTarget);
	}
	else
	{
		return 0.0f;
	}
}

Vector3f MatterRootSegment::GetTipPosition() const
{
	return (Transform.GetLookAt() * _Length + Transform.GetPosition());
}

void MatterRootSegment::SetSourceRootSegment(const MatterRootSegment& SourceRootSegment)
{
	Transform.SetPosition(SourceRootSegment.GetTipPosition());
	InRadius = SourceRootSegment._OutRadius;
	//Taper = SourceRootSegment.Taper + (1.0f - SourceRootSegment.Taper) * 0.1f;
}