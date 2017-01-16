#include "Animation_Aim.h"

#include "Matter.h"
#include "MatterRootSegment.h"

Animation_Aim::Animation_Aim(Vector3f Target)
{
	OutputIdx_AimVector = RegisterOutput_Vec3(Out_AimVector);
	Out_AimVector = Target;
}


Animation_Aim::~Animation_Aim()
{
}

bool Animation_Aim::Tick(const double DeltaSeconds)
{
	return !Finished(); //finished immidiately todo: can make this aim over a time
}

bool Animation_Aim::Finished() const
{
	return true;
}