#include "MatterWireCube.h"
#include "MeshWireCube.h"
#include "Animation_LerpVec3.h"

MatterWireCube::MatterWireCube() :
	Matter("MatterWireCube", Counter<MatterWireCube>::Count())
{
	MeshWireCube* Cube = new MeshWireCube();
	Vector4f CubeColor;

	float r = static_cast<float>(rand() % 1000) / 1000.0f;
	float g = static_cast<float>(rand() % 1000) / 1000.0f;
	float b = static_cast<float>(rand() % 1000) / 1000.0f;
	CubeColor << r, g, b, 0.4f;
	Cube->SetColor(CubeColor);
	Mesh = Cube;
}

MatterWireCube::~MatterWireCube()
{
	delete Mesh;
}