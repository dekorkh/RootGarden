#include "MatterRootCluster.h"
#include "MatterRootTendril.h"

MatterRootCluster::MatterRootCluster() :
	SceneComponent("MatterRootCluster", Counter<MatterRootCluster>::Count()),
	NumTendrils(0),
	MaxTendrils(16)
{
	while (NumTendrils < MaxTendrils)
		SpawnTendril();
}

void MatterRootCluster::SpawnTendril()
{
	NumTendrils++;
	MatterRootTendril* newRootTendril = new MatterRootTendril();
	AddChild(newRootTendril);
}