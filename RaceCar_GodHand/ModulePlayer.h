#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

struct PhysVehicle3D;

#define MAX_ACCELERATION 750.0f
#define MAX_BACKWARDS_ACCELERATION -375.0f
#define TURN_DEGREES 15.0f * DEGTORAD
#define BRAKE_POWER 1000.0f

class ModulePlayer : public Module
{
public:
	ModulePlayer(Application* app, bool start_enabled = true);
	virtual ~ModulePlayer();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	float GetX();
	float GetY();
	float GetZ();
	btQuaternion GetRotationQuaternion();

public:
	PhysVehicle3D* vehicle;
	btTransform initialPos;
	float turn;
	float acceleration;
	float brake;
};