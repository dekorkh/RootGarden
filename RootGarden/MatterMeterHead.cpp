#include "MatterMeterHead.h"
#include "MeshRectangle.h"

MatterMeterHead::MatterMeterHead() :
	Matter("MatterMeterHead", Counter<MatterMeterHead>::Count()),
	HeadPosition(0.0f),
	HeadThickness(0.02f)
{
	Mesh = new MeshRectangle();
}

MatterMeterHead::~MatterMeterHead()
{
	delete Mesh;
}

void MatterMeterHead::Build()
{
	SceneComponent::Build();
	HeadPosition = static_cast<float>(fmin(fmax(0.0, HeadPosition), 1.0));
	HeadThickness = static_cast<float>(fmin(fmax(0.0, HeadThickness), 1.0));

	SetScale(LastScale);
	SetPosition(LastPosition);
}

void MatterMeterHead::SetScale(const Vector3f& NewScale)
{
	Vector3f NewScaleHead;
	NewScaleHead << NewScale(0), NewScale(1) * HeadThickness, NewScale(2);
	//NewScaleHead << NewScale(0), NewScale(1), NewScale(2);

	// Head pos y 0 is bottom of a -1 to 1 (NDC bottom to top) and OFFSET UPWARD by half the height of the head's scale so its bottom lays on the bottom of the meter.
	HeadPosY0 = -1.0f * NewScale(1) + NewScaleHead(1) * 0.5f;
	// Head pos y 1 is the same for the top.
	HeadPosY1 = NewScale(1) - NewScaleHead(1) * 0.5f;

	// Store last scale so that if HeadThickness changes we can restore it.
	LastScale = NewScale;

	Matter::SetScale(NewScaleHead);
}

void MatterMeterHead::SetPosition(const Vector3f& NewPosition)
{
	LastPosition = NewPosition;
	float HeadPosYOffset = HeadPosY0 * (1.0f - HeadPosition) + HeadPosY1 * HeadPosition;
	Vector3f NewPositionHead;
	NewPositionHead << NewPosition(0), NewPosition(1) + HeadPosYOffset, NewPosition(2);
	
	Matter::SetPosition(NewPositionHead);
}