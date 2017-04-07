#include "MatterCircle.h"
#define _USE_MATH_DEFINES
#include "math.h"

MatterCircle::MatterCircle() :
	Matter("MatterCircle", Counter<MatterCircle>::Count())
{
	Name = "MatterCircle";
	Mesh = new MeshCircle();
	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");
}

MatterCircle::~MatterCircle()
{
	delete Mesh;
}

void MatterCircle::SetColors(const Vector4f& InColorInner, const Vector4f& InColorOuter)
{
	static_cast<MeshCircle*>(Mesh)->SetColors(InColorInner, InColorOuter);
}