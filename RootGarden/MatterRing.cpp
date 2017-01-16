#include "MatterRing.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "MeshRing.h"

MatterRing::MatterRing() :
	Matter("MatterRing", Counter<MatterRing>::Count())
{
	Name = "MatterRing";
	Mesh = new MeshRing();
}

MatterRing::~MatterRing()
{
	delete Mesh;
}