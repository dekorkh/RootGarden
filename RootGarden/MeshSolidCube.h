#pragma once

#include "Mesh.h"

class MeshSolidCube : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		MeshSolidCube();
	~MeshSolidCube() override;

	void SetColor(const Vector4f& Color);

protected:
	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_Indices() override;

private:
	Vector4f Color;
};
