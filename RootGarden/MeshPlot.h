#pragma once

#include "Mesh.h"

struct MeshPlotGridInfo
{
	GLuint numPosX;
	GLuint numNegX;
	GLuint numPosY;
	GLuint numNegY;
	GLuint numAllX;
	GLuint numAllY;
	
	void Compute(Vector3f const &InCenter, GLfloat const InWidth, GLfloat const InHeight, GLfloat const InDivSpacingX, GLfloat const InDivSpacingY)
	{
		numPosX = static_cast<GLint>(floorf(abs(InWidth  *  0.5f - InCenter.x()) / InDivSpacingX));
		numNegX = static_cast<GLint>(floorf(abs(InWidth  * -0.5f - InCenter.x()) / InDivSpacingX));
		numPosY = static_cast<GLint>(floorf(abs(InHeight *  0.5f - InCenter.y()) / InDivSpacingY));
		numNegY = static_cast<GLint>(floorf(abs(InHeight * -0.5f - InCenter.y()) / InDivSpacingY));
		numAllX = numPosX + numNegX;
		numAllY = numPosY + numNegY;
	}
};

class MeshPlot : public Mesh
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
		MeshPlot();
	~MeshPlot() override;

	void SetColor(const Vector4f& Color);
	void SetCenter(const Vector3f& Center);
	void SetUnitScale(Vector3f const &InUnitScale);

protected:
	void GenerateMesh_Positions() override;
	void GenerateMesh_Colors() override;
	void GenerateMesh_TexCoords() override;
	void GenerateMesh_Indices() override;

	Vector4f Color;
	Vector3f Center;
	GLfloat Width;
	GLfloat Height;
	GLfloat DivSpacing_X;
	GLfloat DivSpacing_Y;
	GLfloat Division_Height;
};
