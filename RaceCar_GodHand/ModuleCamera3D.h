#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"

#include "Bullet/include/btBulletDynamicsCommon.h"

enum CameraType
{
	NORMAL,
	EAGLE,
	DEBUG,
	END_OF_LIST
};

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void FollowPlayer();
	void Move(const vec3 &Movement);
	float* GetViewMatrix();
	vec3 ToEulerAngles(btQuaternion q);

private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	float playerDistance;
	float playerVerticalAngle;
	/*float playerHorizontalAngle;*/

	//Player Camera
	CameraType cameraType;

private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};