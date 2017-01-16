#include "CompGaugeVertical.h"
#include "Animation_FrameTime.h"

#include "MatterMeter.h"
#include "MatterMeterHead.h"

#define _USE_MATH_DEFINES
#include "math.h"

CompGaugeVertical::CompGaugeVertical()
{
	pMeter = new MatterMeter();
	pHead = new MatterMeterHead();
	
	int InputIdx_Position = pHead->RegisterInput_Float(pHead->HeadPosition);
	pHead->InputMap[InputIdx_Position]->InputEffects.bComponentParameters = true;
	Animation_FrameTime* pAnimation_FrameTime = new Animation_FrameTime(0.05, 60);
	pAnimation_FrameTime->LinkOutput(pAnimation_FrameTime->OutputIdx_Out, InputIdx_Position, EOperationType::OVERRIDE);
	pHead->AddAnimation(pAnimation_FrameTime);
	
	AddChild(pMeter);
	AddChild(pHead);
}

void CompGaugeVertical::SetScale(const Vector3f& NewScale)
{
	pMeter->SetScale(NewScale);
	pHead->SetScale(NewScale);
}

void CompGaugeVertical::SetPosition(const Vector3f& NewPosition)
{
	pMeter->SetPosition(NewPosition);
	pHead->SetPosition(NewPosition);
}