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

void MatterRootCluster::Tick(double DeltaSeconds)
{
	SceneComponent::Tick(DeltaSeconds); //Ticks children	
}

void MatterRootCluster::SpawnTendril()
{
	NumTendrils++;
	MatterRootTendril* newRootTendril = new MatterRootTendril();
	AddChild(newRootTendril);
}