#include "MatterSolidBeveledCube.h"
#include "MeshSolidBeveledCube.h"
#include "Animation_LerpVec3.h"

MatterSolidBeveledCube::MatterSolidBeveledCube() :
	Matter("MatterSolidBeveledCube", Counter<MatterSolidBeveledCube>::Count())
{
	MeshSolidBeveledCube* Cube = new MeshSolidBeveledCube();
	Vector4f CubeColor;

	float r = static_cast<float>(rand() % 1000) / 1000.0f;
	float g = static_cast<float>(rand() % 1000) / 1000.0f;
	float b = static_cast<float>(rand() % 1000) / 1000.0f;
	CubeColor << 1.0, 1.0, 1.0, 1.0f;
	Cube->SetColor(CubeColor);
	Mesh = Cube;
	
	pShaderProgram = ShaderManager::GetShaderManager()->GetShaderProgramByName("basic_prog");
}

MatterSolidBeveledCube::~MatterSolidBeveledCube()
{
	delete Mesh;
}