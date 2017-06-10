
#include "Camera.h"

#define _USE_MATH_DEFINES
#include "math.h"

Camera::Camera() :
	ElapsedSeconds(0.0)
{
}

Camera::~Camera()
{

}

void Camera::Tick(double DeltaSeconds)
{
	// TO-DO -- Make all of this not dirty in "Camera Rework"
	Vector3f CameraPosition;
	CameraPosition << 0.0f, 0.0f, -1.0f;
	Transform.SetPosition(CameraPosition);
	Vector3f CameraLookAt = {0.0f, 0.0f, 1.0f};
	Transform.SetLookAt(CameraLookAt);

	/*
	GLfloat RotateSpeed = 0.05f; //revolutions per second
	ElapsedSeconds += DeltaSeconds;
	float x = static_cast<float>(cos(ElapsedSeconds * RotateSpeed * M_PI * 2));
	float z = static_cast<float>(sin(ElapsedSeconds * RotateSpeed * M_PI * 2));
	Vector3f CameraPosition;
	CameraPosition << x, 0.5f, z;
	Transform.SetPosition(CameraPosition);
	Vector3f CameraLookAt = Vector3f::Constant(0.0f) - CameraPosition;
	Transform.SetLookAt(CameraLookAt);
	*/
}