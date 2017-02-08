#include "SceneComponent.h"
#include "AnimationList.h"
#include "GameStats.h"
#include <time.h>

AnimationList::AnimationList(SceneComponent& InSceneComponent) : 
	SequenceIndex(0),
	rSceneComponent(InSceneComponent)
{
}


AnimationList::~AnimationList()
{
}

void AnimationList::Tick(double DeltaSeconds)
{
	clock_t Clock_Animation = clock();
	if (SequenceIndex < SequenceList.size())
	{
		bool result = SequenceList.at(SequenceIndex)->Tick(DeltaSeconds);
		SequenceList.at(SequenceIndex)->UpdateOutputs(rSceneComponent);
		if (!result)
		{ // ENTER ANIMATION FINISHED
			if (SequenceList.at(SequenceIndex)->bLoop)
			{ // IF LOOP, PLAY AGAIN
				ResetAnimation(SequenceIndex);
			}
			else
			{ // MOVE ON TO NEXT ANIM
				SequenceIndex++;
			}
		} // EXIT ANIMATION FINISHED
	}
	GameStats::GetGameStats()->pThisFrame->Ms_TickAnimation->Increment(TICKS_TO_MS(clock() - Clock_Animation));
}

void AnimationList::Add(Animation* Anim)
{
	SequenceList.insert(SequenceList.end(), Anim);
}

void AnimationList::SetSequenceIndex(int Index)
{
	SequenceIndex = Index;
}

void AnimationList::ResetAnimation(int Index)
{
	SequenceList[Index]->Reset();
}