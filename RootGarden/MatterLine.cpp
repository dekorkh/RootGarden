#include "MatterLine.h"
#include "MeshLine.h"

MatterLine::MatterLine() :
	Matter("MatterLine", Counter<MatterLine>::Count())
{
	Mesh = new MeshLine();

	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("msaa_prog");
}

MatterLine::~MatterLine()
{
	delete Mesh;
}

void MatterLine::SetColor(Vector4f const &InColor)
{
}

void MatterLine::SetPositions(vector<GLfloat> &InPositions)
{
	static_cast<MeshLine*>(Mesh)->SetPositions(InPositions);
}