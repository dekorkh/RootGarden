#pragma once

#include "Mesh.h"

class MeshRing : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	MeshRing();
	~MeshRing();

	void SetNumSegments(GLint InNumSegments);
	void SetColor(const Vector4f& InColor);

protected:
	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_Indices() override;

private:
	GLint NumSegments;
	GLfloat Thickness;
	Vector4f Color;
};
