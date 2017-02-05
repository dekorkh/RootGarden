#pragma once

#include "Mesh_Instanced.h"

class MeshRectangle_Instanced : public Mesh_Instanced
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	MeshRectangle_Instanced(Matter const &InParentMatter);
	~MeshRectangle_Instanced() override;

	void SetColor(const Vector4f& Color);

protected:
	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_Indices() override;

private:
	Vector4f Color;
};
