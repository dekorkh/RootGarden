#pragma once
#include "stdlib.h"

#include "Eigen/StdVector"
#include "Eigen/Dense"
#include "TransformInfo.h"
#include "Mesh.h"
#include "SceneComponent.h"
#include "ShaderManager.h"

#define _USE_MATH_DEFINES
#include "math.h"

#define MAX_NAME_LEN 256

using namespace std;
using namespace Eigen;

class Matter: public SceneComponent
{
public:
	Matter(const string& InName, const int InSerial);
	~Matter() override;

	virtual void SetScale(const Vector3f& InScale);
	virtual void SetPosition(const Vector3f& InPosition);
	virtual void SetLookAt(const Vector3f& InLookAt);
	virtual void SetLookAtTarget(const Vector3f& InTarget);
	virtual void AddRotation(const float Deg);

	/* SceneComponent Interface BEGIN */
	void ProcessInputEffects(TInputEffects const *InputEffects) override;
	/* SceneComponent Interface END */

	virtual void UpdateMesh(GenerateMeshResult *OutGenerateMeshResult);
	virtual void Draw();

	/* IsMeshDirty 
	/ Return true if any mesh data is dirty.  Helps cull meshes that don't need update from being dispatched to threads in main.
	*/
	bool IsMeshDirty();

	/* IsModelMatrixDirty
	/ Return true if the model matrix is dirty.  Helps cull meshes that don't need update from being dispatched to threads in main.
	*/
	bool IsModelMatrixDirty();

	/* UpdateModelMatrix
	/ Make the transform compute a new modelmatrix.
	*/
	void UpdateModelMatrix();

	/* IsUniformDataDirty
	/	Return true if uniform data is dirty. Helps cull uniform updates.
	*/
	bool IsUniformDataDirty();

	/* UpdateUniformData
	/	Do update to uniform data.
	*/
	virtual void UpdateUniformData();

	ShaderProgram *ShaderProgram;

protected:
	TransformInfo Transform;
	Mesh* Mesh;
	
private:
	void Initialize();
	bool bDirtyModelMatrix;
	bool bDirtyUniformData;
};