#pragma once

#include "Mesh.h"

class MeshLine : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		MeshLine();
	~MeshLine() override;

	void SetColor(const Vector4f& Color);

	void SetPositions(vector<GLfloat> & InPositions);

protected:
	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_TexCoords() override;
	void GenerateMesh_Indices() override;

private:
	Vector4f Color;
};
