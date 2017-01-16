#pragma once

#include "Mesh.h"

using namespace Eigen;

class MeshTrapezoid : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	MeshTrapezoid();
	~MeshTrapezoid();

	void SetBaseWidth(float w);
	void SetTipWidth(float w);
	void SetLength(float l);

protected:
	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_Indices() override;

private:
	float Rotation;
	float BaseWidth;
	float TipWidth;
	float Length;
	Vector4f Color;
	
};	

