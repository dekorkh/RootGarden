#pragma once

#include "Eigen/Dense"
#include <map>
#include <vector>

using namespace Eigen;
using namespace std;

class SceneComponent;

enum EValueType { VALUE_FLOAT = 0, VALUE_VEC3, VALUE_BOOL };
enum EOperationType { OVERRIDE = 0, ACCUMULATE };

struct OutputLink
{
	// The index of an input this animation effects.
	int InputIndex;
	// How the value is applied via this link (same output can effect different inputs differently
	// If ACCUMULATE the input values will accumulate each update instead of OVERRIDE previous value
	EOperationType OpType;
};

struct Output
{
	/* Output
	Used to send one output value of an animation to the SceneComponent which uses the 
	list of LinkInputIndices to update it's properties. 	Created and destroyed by an animation.  
	*/
	EValueType Type;
	float *ValueFloat;
	Vector3f *ValueVec3;
	bool *ValueBool;
	vector<OutputLink*> Links;	//Indices used by SceneComponent::ProcessOutput() to update the appropriate property.
};

class Animation
{
public:
	Animation();
	virtual ~Animation();

	virtual bool Tick(const double DeltaSeconds);	//Tick the animation, return false if its complete

	// Return whether the animation has completed
	virtual bool Finished() const { return false; };

	/* RegisterOutput_Float
	Register a float property as an output by adding it into the OutputMap and return it's 
	index/address.
	Attributes:
	InProperty	-	Reference to the float property to be registered.
	*/
	int RegisterOutput_Float(float &InProperty);

	/* RegisterOutput_Vec3
	Register a Vector3f property as an output by adding it into the OutputMap and return it's
	index/address.
	Attributes:
	InProperty	-	Reference to the Vector3f property to be registered.
	*/
	int RegisterOutput_Vec3(Vector3f &InProperty);

	/* RegisterOutput_Bool
	Register a Vector3f property as an output by adding it into the OutputMap and return it's
	index/address.
	Attributes:
	InProperty	-	Reference to the Vector3f property to be registered.
	*/
	int RegisterOutput_Bool(bool &InProperty);

	/* LinkOutput
	Adds the InputIndex to the output at OutputIndex.  SceneComponent's UpdateInputs() will be
	called with each Output and those containing the input indices on the component will be 
	updated.
	Attributes:
		OutputIndex	-	Index (map address) of the output.
		InputIndex	-	Index (map address) of the input.
		OpType		-	How the value from output is to be applied (Override/Accumulate)
	*/
	void LinkOutput(int OutputIndex, int InputIndex, EOperationType OpType);

	/* UpdateOutputs
	Calls UpdateInputs on the attached scenecomponent for every output in the output map.
	Attributes:
		InSceneComponent	-	The scene component to which this animation is belongs to.
	*/
	void UpdateOutputs(SceneComponent& InSceneComponent) const;

	/* ResetTime 
	Sets the ElapsedSeconds to 0 - if the animation uses time this rewindes it. */
	void Reset();


protected:
	double ElapsedSeconds;

	map<const int, Output*> OutputMap;
};