#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "p2Point.h"
#include "Globals.h"
#include "Primitive.h"

#define MAX_ELEMENTS 1024

struct PhysBody3D;
struct PhysMotor3D;

// FROM EAGLE CAM POV
enum class CurveType
{
	BOTTOM_RIGHT,
	BOTTOM_LEFT,
	TOP_LEFT,
	TOP_RIGHT
};

// FROM EAGLE CAM POV
enum class RoadType
{
	HORIZONTAL_LEFT,
	HORIZONTAL_RIGHT,
	VERTICAL_UP,
	VERTICAL_DOWN
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update(float dt);
    update_status PostUpdate();
	bool CleanUp();

	void BuildCircuit();
	void CreateCurve(p2Point<float> circleCenter, float radious, float referenceAngle, float sizePilon, float heightPilon, int numPilons, float pilonDistance, CurveType type);
	float CreateRoad(p2Point<float> init, float sizePilon, float heightPilon, int numPilons, float pilonDistance, float halfRoad, RoadType type);
	void CreateRamp(p2Point<float> position, float height, float length, float angle, RoadType type);
	void CreateBorders();

	void OnCollision(PhysBody3D* body1, PhysBody3D* body2);

public:
	/*
	PhysBody3D* pb_snake[MAX_SNAKE];
	Sphere s_snake[MAX_SNAKE];

	PhysBody3D* pb_snake2[MAX_SNAKE];
	Sphere s_snake2[MAX_SNAKE];
	*/

    p2DynArray<Primitive*> c_circuit;

	PhysBody3D* pb_chassis;
	Cube p_chassis;

	PhysBody3D* pb_wheel;
	Cylinder p_wheel;

	PhysBody3D* pb_wheel2;
	Cylinder p_wheel2;

	PhysMotor3D* left_wheel;
	PhysMotor3D* right_wheel;
};
