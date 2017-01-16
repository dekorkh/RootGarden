#pragma once

#include "Mesh.h"

class MeshCircle : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	MeshCircle();
	~MeshCircle();

	void SetNumSides(GLint InNumSides);
	void SetColors(const Vector4f& InColorOuter, const Vector4f& InColorInner);

	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_Indices() override;

private:
	GLint NumSides;
	Vector4f ColorOuter;
	Vector4f ColorInner;
};