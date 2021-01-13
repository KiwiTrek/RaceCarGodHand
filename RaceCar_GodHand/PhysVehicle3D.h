#pragma once

#include "PhysBody3D.h"
#include "glmath.h"

class btRaycastVehicle;
struct PhysBody3D;

struct Wheel
{
	vec3 connection; // origin of the ray. Must come from within the chassis
	vec3 direction; 
	vec3 axis;
	float suspensionRestLength; // max length for suspension in meters
	float radius;
	float width;
	bool front; // is front wheel ?
	bool drive; // does this wheel received engine power ?
	bool brake; // does breakes affect this wheel ?
	bool steering; // does this wheel turns ?
};

struct VehicleInfo
{
	~VehicleInfo();
	
	//Car parts
	vec3 bump_front_left_size;
	vec3 bump_front_left_offset;
	vec3 bump_front_right_size;
	vec3 bump_front_right_offset;
	vec3 bump_front_size;
	vec3 bump_front_offset;

	vec3 bump_left_size;
	vec3 bump_left_offset;
	vec3 joint_left_one_size;
	vec3 joint_left_one_offset;
	vec3 joint_left_two_size;
	vec3 joint_left_two_offset;

	vec3 bump_right_size;
	vec3 bump_right_offset;
	vec3 joint_right_one_size;
	vec3 joint_right_one_offset;
	vec3 joint_right_two_size;
	vec3 joint_right_two_offset;

	vec3 front_size;
	vec3 front_offset;
	vec3 joint_front_one_size;
	vec3 joint_front_one_offset;
	vec3 joint_front_two_size;
	vec3 joint_front_two_offset;

	float cabin_radious;
	vec3 cabin_offset;

	vec3 motor_size;
	vec3 motor_offset;

	vec3 motor_tube_top_left_size;
	vec3 motor_tube_top_left_offset;
	vec3 motor_tube_top_right_size;
	vec3 motor_tube_top_right_offset;

	vec3 chassis_size;
	vec3 chassis_offset;
	float mass;
	float suspensionStiffness; // default to 5.88 / 10.0 offroad / 50.0 sports car / 200.0 F1 car
	float suspensionCompression; // default to 0.83
	float suspensionDamping; // default to 0.88 / 0..1 0 bounces / 1 rigid / recommended 0.1...0.3
	float maxSuspensionTravelCm; // default to 500 cm suspension can be compressed
	float frictionSlip; // defaults to 10.5 / friction with the ground. 0.8 should be good but high values feels better (kart 1000.0)
	float maxSuspensionForce; // defaults to 6000 / max force to the chassis

	Wheel* wheels;
	int num_wheels;
};


struct PhysVehicle3D : public PhysBody3D
{
public:
	PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info);
	~PhysVehicle3D();

	void Render();
	void DebugRender();
	void ApplyEngineForce(float force);
	void Brake(float force);
	void Turn(float degrees);
	float GetKmh() const;
public:

	VehicleInfo info;
	btRaycastVehicle* vehicle;
};