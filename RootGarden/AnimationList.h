#pragma once

#include "Animation.h"
#include "vector"

using namespace std;

class SceneComponent;

class AnimationList
{
public:
	AnimationList(SceneComponent& InSceneComponent);
	~AnimationList();

	virtual void Tick(double DeltaSeconds);
	void Add(Animation* Anim);
	void SetSequenceIndex(int Index);	//Select a sequence by index
	void ResetAnimation(int Index);

private:
	vector<Animation*> SequenceList;
	uint16_t SequenceIndex;	//The index to the currently active animation

	SceneComponent& rSceneComponent;
};

