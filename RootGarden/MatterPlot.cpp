#include "MatterPlot.h"
#include "MeshPlot.h"

MatterPlot::MatterPlot() :
	Matter("MatterPlot", Counter<MatterPlot>::Count()),
	UnitScale(1.0f)
{
	MeshPlot* pMeshPlot = new MeshPlot();
	
	Mesh = pMeshPlot;

	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");
}

MatterPlot::~MatterPlot()
{
	delete Mesh;
}

void MatterPlot::SetColor(Vector4f const &InColor)
{
	static_cast<MeshPlot*>(Mesh)->SetColor(InColor);
}

void MatterPlot::SetCenter(Vector3f const &InCenter)
{
	static_cast<MeshPlot*>(Mesh)->SetCenter(InCenter);
}

void MatterPlot::SetUnitScale(Vector3f const &InUnitScale)
{
	static_cast<MeshPlot*>(Mesh)->SetUnitScale(InUnitScale);
}
