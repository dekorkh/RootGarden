#include "MatterSolidCube.h"
#include "MeshSolidCube.h"
#include "Animation_LerpVec3.h"

MatterSolidCube::MatterSolidCube() :
	Matter("MatterSolidCube", Counter<MatterSolidCube>::Count())
{
	MeshSolidCube* Cube = new MeshSolidCube();
	Vector4f CubeColor;

	float r = static_cast<float>(rand() % 1000) / 1000.0f;
	float g = static_cast<float>(rand() % 1000) / 1000.0f;
	float b = static_cast<float>(rand() % 1000) / 1000.0f;
	CubeColor << r, g, b, 1.0f;
	Cube->SetColor(CubeColor);
	Mesh = Cube;
}

MatterSolidCube::~MatterSolidCube()
{
	delete Mesh;
}