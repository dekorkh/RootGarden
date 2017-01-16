#include "MatterMeter.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include "MeshMeter.h"

MatterMeter::MatterMeter()
	: Matter("MatterMeter", Counter<MatterMeter>::Count())
{
	Name = "MatterMeter";
	Mesh = new MeshMeter();
}

MatterMeter::~MatterMeter()
{
	delete Mesh;
}