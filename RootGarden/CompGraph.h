#pragma once

#include "SceneComponent.h"

class Animation_LerpVec3;
class MatterPlot;
class MatterLine;

class CompGraph : public Counter<CompGraph>, public SceneComponent
{
public:
	CompGraph();
	~CompGraph() override;

	void Build() override;
	void AddData(vector<GLfloat> &ContigousXYPositions, Vector4f const &Color = Vector4f::Constant(1.0f));

	MatterPlot *Plot;
	MatterLine *Data;

	GLfloat X_Neg;
	GLfloat X_Pos;
	GLfloat Y_Neg;
	GLfloat Y_Pos;
	GLfloat DivisionSpacing_Major;
	Vector3f UnitScale;
	vector<MatterLine*> DataArray;
};
