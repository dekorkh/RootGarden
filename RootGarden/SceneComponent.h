#pragma once

#include "Eigen/StdVector"
#include <map>
#include "Eigen/Dense"
#include "Animation.h"
#include "AnimationList.h"

using namespace std;
using namespace Eigen;

//-----------------------------------

template<typename T>
class Counter {
	/* A curiously recurring template pattern.*/
public:
	Counter() { ++count; };
	Counter(const Counter&) { ++count; };
	virtual ~Counter() { --count; };

	static int Count() { return count; };

private:
	static int count;
};

template<typename T>
int Counter<T>::count = 0;

//-----------------------------------

struct TInputEffects { 
	TInputEffects() :
		bVertPositions(false),
		bVertColors(false),
		bVertIndices(false),
		bComponentParameters(false)
	{};
	bool bVertPositions;
	bool bVertColors;
	bool bVertIndices;
	bool bComponentParameters;
};

//-----------------------------------

struct Input
{
	EValueType Type;
	float *ValueFloat;
	Vector3f *ValueVec3;
	bool *ValueBool;
	// Updates to this input can be made to cause different vertex attributes to regenerate.
	TInputEffects InputEffects;
};

//-----------------------------------

class SceneComponent
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	SceneComponent();
	SceneComponent(const string& InName, const int InSerial);
	virtual ~SceneComponent();

	string Name;
	vector<SceneComponent*> Children;
	SceneComponent* Parent;

	void AddChild(SceneComponent* ChildComponent);

	virtual void Tick(double deltaSeconds);

	virtual void HandleInput(int Key, int x, int y, bool down);

	// Recursively gather all the Children descendants
	void GatherChildren(vector<SceneComponent*>* OutChildren);
	
	void AddAnimation(Animation* pAnimation_In);
	void SelectAnimation(int SequenceIndex);	//Play an animation by index

	/* RegisterInput_Float
	/	Register an attribute as an input and return its input index.
	/	Arguments:
	/	Attribute	-	Address to a float attribute to register as an input.
	*/
	int RegisterInput_Float(float &Attribute);

	/* RegisterInput_Vec3
	/	Register an attribute as an input and return its input index.
	/	Arguments:
	/	Attribute	-	Address to a vec3 attribute to register as an input.
	*/
	int RegisterInput_Vec3(Vector3f &Attribute);

	/* RegisterInput_Bool
	/	Register an attribute as an input and return its input index.
	/	Arguments:
	/	Attribute	-	Address to a Bool attribute to register as an input.
	*/
	int RegisterInput_Bool(bool &Attribute);

	/* UpdateInputs
	/	Using information in Output, updates any relevant registered
	/	inputs (attributes registered as inputs).
	/	Arguments:
	/	InOutput	-	Contains a list of inputs to update, new value, and value type.
	*/
	void UpdateInputs(Output* InOutput);

	/* ProcessInputEffects
	/	It's important to refresh only data that was effected by an input change for performance.
	/	Which data is effected by which inputs is thus broken up and this function is meant to
	/	route the appropriate input effects to the code that updates only the relevant data.
	/	The number of effects and data processes is arbitrary so it's meant to be overriden as needed.
	*/
	virtual void ProcessInputEffects(TInputEffects const *InputEffects);

	/* Build
	/ Update anything that depends on component's parameters.
	*/
	virtual void Build();

	/* IsDirtyComponentParameters
	/	True if change to this component's parameters requires a rebuild.
	*/
	bool IsDirtyComponentParameters();

	map<const int, Input*> InputMap;

	static vector<SceneComponent*>* GetPrims();

protected:
	bool bDirtyComponentParameters;

private:
	void Initialize();

	AnimationList* pAnimationList;
	
	// All the scene components in existences.
	static vector<SceneComponent*> AllPrims;
};