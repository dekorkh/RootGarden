#pragma once

#include "Mesh_Instanced.h"

// Debugging Mesh_Instanced not drawing:
//	Is draw being called?
//	-Yes:
//			Are primitive type, position, color, index data, and model matrix data valid?
//			To test use bUseDebug____  flags that will cause the mesh to use known test data statically provided by the class.
//

class MeshCircle_Instanced : public Mesh_Instanced
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	MeshCircle_Instanced(Matter const &InMatter);
	~MeshCircle_Instanced() override;

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