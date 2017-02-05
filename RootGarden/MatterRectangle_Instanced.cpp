#include "MatterRectangle_Instanced.h"
#include "MeshRectangle_Instanced.h"


MatterRectangle_Instanced::MatterRectangle_Instanced()
	: Matter("MatterRectangle_Instanced", Counter<MatterRectangle_Instanced>::Count())
{
	Mesh = new MeshRectangle_Instanced(*this);
}


MatterRectangle_Instanced::~MatterRectangle_Instanced()
{
	delete Mesh;
}
