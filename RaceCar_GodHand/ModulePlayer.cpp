#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled), vehicle(NULL)
{
	turn = acceleration = brake = 0.0f;
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
    LOG("Loading player");

    VehicleInfo car;

    // Car properties ----------------------------------------
    car.chassis_size.Set(2, 2, 4);
    car.chassis_offset.Set(0, 1.5, 0);
    car.mass = 500.0f;
    car.suspensionStiffness = 15.88f;
    car.suspensionCompression = 0.83f;
    car.suspensionDamping = 0.88f;
    car.maxSuspensionTravelCm = 1000.0f;
    car.frictionSlip = 50.5;
    car.maxSuspensionForce = 6000.0f;

    // Wheel properties ---------------------------------------
    float connection_height = 1.2f;
    float wheel_radius = 0.6f;
    float wheel_width = 0.5f;
    float suspensionRestLength = 1.2f;

    // Don't change anything below this line ------------------

    float half_width = car.chassis_size.x * 0.5f;
    float half_length = car.chassis_size.z * 0.5f;

    vec3 direction(0, -1, 0);
    vec3 axis(-1, 0, 0);

    car.num_wheels = 4;
    car.wheels = new Wheel[4];

    // FRONT-LEFT ------------------------
    car.wheels[0].connection.Set(half_width - 0.3f * wheel_width, connection_height, half_length - wheel_radius);
    car.wheels[0].direction = direction;
    car.wheels[0].axis = axis;
    car.wheels[0].suspensionRestLength = suspensionRestLength;
    car.wheels[0].radius = wheel_radius;
    car.wheels[0].width = wheel_width;
    car.wheels[0].front = true;
    car.wheels[0].drive = true;
    car.wheels[0].brake = false;
    car.wheels[0].steering = true;

    // FRONT-RIGHT ------------------------
    car.wheels[1].connection.Set(-half_width + 0.3f * wheel_width, connection_height, half_length - wheel_radius);
    car.wheels[1].direction = direction;
    car.wheels[1].axis = axis;
    car.wheels[1].suspensionRestLength = suspensionRestLength;
    car.wheels[1].radius = wheel_radius;
    car.wheels[1].width = wheel_width;
    car.wheels[1].front = true;
    car.wheels[1].drive = true;
    car.wheels[1].brake = false;
    car.wheels[1].steering = true;

    // REAR-LEFT ------------------------
    car.wheels[2].connection.Set(half_width - 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
    car.wheels[2].direction = direction;
    car.wheels[2].axis = axis;
    car.wheels[2].suspensionRestLength = suspensionRestLength;
    car.wheels[2].radius = wheel_radius;
    car.wheels[2].width = wheel_width;
    car.wheels[2].front = false;
    car.wheels[2].drive = false;
    car.wheels[2].brake = true;
    car.wheels[2].steering = false;

    // REAR-RIGHT ------------------------
    car.wheels[3].connection.Set(-half_width + 0.3f * wheel_width, connection_height, -half_length + wheel_radius);
    car.wheels[3].direction = direction;
    car.wheels[3].axis = axis;
    car.wheels[3].suspensionRestLength = suspensionRestLength;
    car.wheels[3].radius = wheel_radius;
    car.wheels[3].width = wheel_width;
    car.wheels[3].front = false;
    car.wheels[3].drive = false;
    car.wheels[3].brake = true;
    car.wheels[3].steering = false;

    vehicle = App->physics->AddVehicle(car);
    vehicle->SetPos(115, 0, 45);
    return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
	turn = acceleration = brake = 0.0f;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
        if (vehicle->GetKmh() < 0.0)
        {
            brake = BRAKE_POWER;
        }
        if (vehicle->GetKmh() <= 75.0f && App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT);
        {
            acceleration = MAX_ACCELERATION;
        }
	}

	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		if(turn < TURN_DEGREES)
			turn +=  TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		if(turn > -TURN_DEGREES)
			turn -= TURN_DEGREES;
	}

	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		brake = BRAKE_POWER;
        if (vehicle->GetKmh() <= 1.0f)
        {
            brake = 0.0f;
            if (vehicle->GetKmh() >= -25.0f)
            {
                acceleration = MAX_BACKWARDS_ACCELERATION;
            }
        }
	}

    //if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) // do we comment this line? (makes it really stable but there is no sensation of suspension)
    {
        btQuaternion newQuat = GetRotationQuaternion();										//Obtain rotation of car in quaternion form
        vec3 newEulerAngles = App->camera->ToEulerAngles(newQuat);							//Transform quaternion to corresponding Euler angles
        newEulerAngles.x *= RADTODEG;														//Convert to degrees because rotate function needs degrees :)
        newEulerAngles.y *= RADTODEG;
        newEulerAngles.z *= RADTODEG;
        LOG("Angles: %f,%f,%f", newEulerAngles.x, newEulerAngles.y, newEulerAngles.z);
        LOG("Pos: %f,%f,%f", GetX(), GetY(), GetZ());


        if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
        {
            mat4x4 matrix;
            vehicle->GetTransform(&matrix);
            matrix.rotate(180, vec3(1.0f, 0.0f, 1.0f));
            vehicle->SetTransform(&matrix);
        }

        if (abs(newEulerAngles.z) >= 75 && GetY() <= 3)
        {
            // Snap
            //mat4x4 matrix;
            //vehicle->GetTransform(&matrix);
            //matrix.rotate(180, vec3(1.0f, 0.0f, 0.0f));
            //matrix.rotate(newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));
            //vehicle->SetTransform(&matrix);

            // NOT snap
            btScalar x = 0;
            btScalar y = 0;
            btScalar z = 75;
            btVector3 torque(x, y, z);
            vehicle->vehicle->getRigidBody()->applyTorqueImpulse(torque);
            LOG("YAY Z!");
        }
        else if (abs(newEulerAngles.x) >= 75 && GetY() <= 3)
        {
            // Snap
            //mat4x4 matrix;
            //vehicle->GetTransform(&matrix);
            //matrix.rotate(180, vec3(1.0f, 0.0f, 0.0f));
            //matrix.rotate(newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));
            //vehicle->SetTransform(&matrix);

            // NOT snap
            btScalar x = 75;
            btScalar y = 0;
            btScalar z = 0;
            btVector3 torque(x, y, z);
            vehicle->vehicle->getRigidBody()->applyTorqueImpulse(torque);
            LOG("YAY X!");
        }
    }



    LOG("Velocity: %f", vehicle->GetKmh());
	vehicle->ApplyEngineForce(acceleration);
	vehicle->Turn(turn);
	vehicle->Brake(brake);

	vehicle->Render();

	char title[80];
	sprintf_s(title, "%.1f Km/h", vehicle->GetKmh());
	App->window->SetTitle(title);

	return UPDATE_CONTINUE;
}

float ModulePlayer::GetX()
{
	return vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().x();
};
float ModulePlayer::GetY()
{
	return vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().y();
};
float ModulePlayer::GetZ()
{
	return vehicle->vehicle->getRigidBody()->getCenterOfMassPosition().z();
};
btQuaternion ModulePlayer::GetRotationQuaternion()
{
	return vehicle->vehicle->getRigidBody()->getWorldTransform().getRotation();
};