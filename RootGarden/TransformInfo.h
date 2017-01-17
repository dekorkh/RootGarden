#pragma once

#include "GL/glew.h"
#include "Eigen\Dense"
#include "math.h"
#include "ShaderManager.h"

using namespace std;
using namespace Eigen;


class TransformInfo
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW

	TransformInfo();
	~TransformInfo();

	static void ConvertDegToLookAt(const float degrees, Vector3f& OutLookAt); //converts from degrees rotation to look at vector.
	static float ConvertLookAtToDeg(const Vector3f& InLookAt); //converts from look at vector to degrees

	/* ComputeModelMatrix
	/ Model matrix is the objects translation, rotation, and scale.
	*/
	void ComputeModelMatrix();

	void SetScale(const Vector3f& Scale);
	void SetPosition(const Vector3f& Position);
	void SetLookAt(const Vector3f& LookAt);
	
	Vector3f GetPosition() const;
	Vector3f GetLookAt() const;
	
	void AddRotation(const float Deg);

	const GLfloat* GetModelMatrixDataPtr() const;

private:
	void SetUniform(ShaderProgram const * InShaderProgram);

	Matrix4f ModelMatrix;

	Vector3f Scale;
	Vector3f Position;
	Vector3f LookAt;
};

