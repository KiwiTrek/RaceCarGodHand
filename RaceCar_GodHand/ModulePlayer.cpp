#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "Primitive.h"
#include "PhysVehicle3D.h"
#include "PhysBody3D.h"

#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"

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
    car.chassis_size.Set(1.15f, 1, 4);
    car.chassis_offset.Set(0, 0.75, 0);

    //Bump left
    car.bump_left_size.Set(0.25, 0.5, 2.15f);
    car.bump_left_offset.Set(0.9f, 0.75, -0.10f);

    car.joint_left_one_size.Set(0.75, 0.1, 0.25);
    car.joint_left_one_offset.Set(0.5, 0.75, 0.35);

    car.joint_left_two_size.Set(0.75, 0.1, 0.25);
    car.joint_left_two_offset.Set(0.5, 0.75, -0.65f);

    //Bump right
    car.bump_right_size.Set(0.25, 0.5, 2.15f);
    car.bump_right_offset.Set(-0.9f, 0.75, -0.15f);

    car.joint_right_one_size.Set(0.75, 0.1, 0.25);
    car.joint_right_one_offset.Set(-0.5, 0.75, 0.35);

    car.joint_right_two_size.Set(0.75, 0.1, 0.25);
    car.joint_right_two_offset.Set(-0.5, 0.75, -0.65f);

    //Bump front
    car.bump_front_size.Set(1.8f, 0.5, 0.25f);
    car.bump_front_offset.Set(0, 0.65, 2.4f);
    car.bump_front_left_size.Set(0.25, 0.5, 0.5);
    car.bump_front_left_offset.Set(0.9f, 0.65, 2.1f);
    car.bump_front_right_size.Set(0.25, 0.5, 0.5);
    car.bump_front_right_offset.Set(-0.9f, 0.65, 2.1f);

    car.joint_front_one_size.Set(0.2, 0.2, 0.65f);
    car.joint_front_one_offset.Set(0.3, 0.65, 2.0f);

    car.joint_front_two_size.Set(0.2, 0.2, 0.65f);
    car.joint_front_two_offset.Set(-0.3, 0.65, 2.0f);

    //Cabin
    car.cabin_radious = 0.5f;
    car.cabin_offset.Set(0, 1.25f, 0);

    //Motor
    car.motor_size.Set(0.75, 0.75, 0.75);
    car.motor_offset.Set(0, 1.15f, -1.25f);

    car.motor_tube_top_left_size.Set(0.2f, 0.8, 0.2f);
    car.motor_tube_top_left_offset.Set(0.45, 1.3, -1.75f);

    car.motor_tube_top_right_size.Set(0.2f, 0.8, 0.2f);
    car.motor_tube_top_right_offset.Set(-0.45, 1.3, -1.75f);

    car.mass = 500.0f;
    car.suspensionStiffness = 50.88f;
    car.suspensionCompression = 0.83f;
    car.suspensionDamping = 0.88f;
    car.maxSuspensionTravelCm = 1000.0f;
    car.frictionSlip = 100.5;
    car.maxSuspensionForce = 6000.0f;

    // Wheel properties ---------------------------------------
    float connection_height = 1.2f;
    float wheel_radius = 0.4f;
    float wheel_width = 0.5f;
    float suspensionRestLength = 1.2f;

    // Don't change anything below this line ------------------

    float half_width = (car.chassis_size.x * 0.5f) + 0.25f;
    float half_length = car.chassis_size.z * 0.5f;

    vec3 direction(0, -1, 0);
    vec3 axis(-1, 0, 0);

    car.num_wheels = 4;
    car.wheels = new Wheel[4];

    // FRONT-LEFT ------------------------
    car.wheels[0].connection.Set(half_width - 0.1f * wheel_width, connection_height, half_length - wheel_radius - 0.25f);
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
    car.wheels[1].connection.Set(-half_width + 0.1f * wheel_width, connection_height, half_length - wheel_radius - 0.25f);
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
    car.wheels[2].connection.Set(half_width - 0.0f * wheel_width, connection_height, -half_length + wheel_radius);
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
    car.wheels[3].connection.Set(-half_width + 0.0f * wheel_width, connection_height, -half_length + wheel_radius);
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
    vehicle->collision_listeners.add(this);

    victory = false;
    lose = false;

    acceleratingChannel = -1;
    accelerationFx = App->audio->LoadFx("Assets/Fx/accel.wav");

    backwardsChannel = -1;
    backwardsFx = App->audio->LoadFx("Assets/Fx/backwards.wav");

    brakeFx = App->audio->LoadFx("Assets/Fx/brake.wav");
    turnFx = App->audio->LoadFx("Assets/Fx/steer.wav");

    ResetPosition();

    isAccelerating = false;
    isGoingBackwards = false;
    isBraking = false;
    isStarting = false;
    isTurningLeft = false;
    isTurningRight = false;

    return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");
    App->audio->UnloadFx(accelerationFx);
    App->audio->UnloadFx(brakeFx);
    App->audio->UnloadFx(turnFx);
    App->audio->UnloadFx(backwardsFx);

	return true;
}

// Update: draw background
update_status ModulePlayer::Update(float dt)
{
    bool debugReset = false;
	turn = acceleration = brake = 0.0f;

    if (!App->scene_intro->start)
    {
        if (!victory && !lose)
        {
            if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
            {
                if (!isAccelerating)
                {
                    acceleratingChannel = App->audio->PlayFx(accelerationFx, -1);
                    isAccelerating = true;
                }
                if (vehicle->GetKmh() < -1.0)
                {
                    brake = BRAKE_POWER;
                    if (!isStarting)
                    {
                        isStarting = true;
                        App->audio->PlayFx(brakeFx);
                    }
                }
                if (vehicle->GetKmh() <= 55.0f && App->input->GetKey(SDL_SCANCODE_LCTRL) != KEY_REPEAT);
                {
                    acceleration = MAX_ACCELERATION;
                }
            }
            else
            {
                if (isAccelerating)
                {
                    App->audio->StopFx(acceleratingChannel);
                    isAccelerating = false;
                }
                if (isStarting)
                {
                    isStarting = false;
                }
            }

            if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_D) != KEY_REPEAT)
            {
                if (turn < TURN_DEGREES)
                    turn += TURN_DEGREES;

                if (!isTurningLeft && !isGoingBackwards)
                {
                    isTurningLeft = true;
                    App->audio->PlayFx(turnFx, 0);
                }
            }
            else
            {
                if (isTurningLeft == true)
                {
                    isTurningLeft = false;
                }
            }

            if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_A) != KEY_REPEAT)
            {
                if (turn > -TURN_DEGREES)
                    turn -= TURN_DEGREES;

                if (!isTurningRight && !isGoingBackwards)
                {
                    isTurningRight = true;
                    App->audio->PlayFx(turnFx, 0);
                }
            }
            else
            {
                if (isTurningRight == true)
                {
                    isTurningRight = false;
                }
            }

            if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
            {
                brake = BRAKE_POWER;
                if (vehicle->GetKmh() <= 1.0f)
                {
                    brake = 0.0f;
                    if (vehicle->GetKmh() >= -25.0f)
                    {
                        acceleration = MAX_BACKWARDS_ACCELERATION;
                        if (!isGoingBackwards)
                        {
                            isGoingBackwards = true;
                            backwardsChannel = App->audio->PlayFx(backwardsFx, -1);
                        }
                    }
                }
                if (!isBraking)
                {
                    isBraking = true;
                    App->audio->PlayFx(brakeFx);
                }
            }
            else
            {
                if (isGoingBackwards)
                {
                    isGoingBackwards = false;
                    App->audio->StopFx(backwardsChannel);
                }
                if (isBraking)
                {
                    isBraking = false;
                }
            }

            if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
            {
                ResetPosition();
                vehicle->Brake(1000.f);
                debugReset = true;
            }

            if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
            {
                victory = true;
            }
            if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
            {
                lose = true;
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


            if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
            {
                mat4x4 matrix;
                vehicle->GetTransform(&matrix);
                matrix.rotate(360, vec3(0.0f, 1.0f, 0.0f));
                matrix.rotate(180, vec3(1.0f, 0.0f, 0.0f));
                vehicle->SetTransform(&matrix);
            }

            //if (abs(newEulerAngles.z) >= 75 && GetY() <= 3)
            //{
            //    // Snap
            //    //mat4x4 matrix;
            //    //vehicle->GetTransform(&matrix);
            //    //matrix.rotate(180, vec3(1.0f, 0.0f, 0.0f));
            //    //matrix.rotate(newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));
            //    //vehicle->SetTransform(&matrix);

            //    // NOT snap
            //    btScalar x = 0;
            //    btScalar y = 0;
            //    btScalar z = 75;
            //    btVector3 torque(x, y, z);
            //    vehicle->vehicle->getRigidBody()->applyTorqueImpulse(torque);
            //    LOG("YAY Z!");
            //}
            if (abs(newEulerAngles.x) >= 75 && GetY() <= 3)
            {
                // Snap
                //mat4x4 matrix;
                //vehicle->GetTransform(&matrix);
                //matrix.rotate(180, vec3(1.0f, 0.0f, 0.0f));
                //matrix.rotate(newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));
                //vehicle->SetTransform(&matrix);

                // NOT snap
                btScalar x = 65;
                btScalar y = 0;
                btScalar z = 65;
                btVector3 torque(x, y, z);
                vehicle->vehicle->getRigidBody()->applyTorqueImpulse(torque);
                LOG("YAY X!");
            }
        }



        LOG("Velocity: %f", vehicle->GetKmh());
        vehicle->ApplyEngineForce(acceleration);
        vehicle->Turn(turn);
        if (!debugReset)
        {
            vehicle->Brake(brake);
        }
        
        if (!App->physics->debug)
        {
            vehicle->Render();
        }
    }

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
void ModulePlayer::ResetPosition()
{
    mat4x4 matrix;
    vehicle->SetPos(115.0f, 0.25f, 45.0f);
    vehicle->GetTransform(&matrix);
    matrix.rotate(360, vec3(0.0f, 1.0f, 0.0f));
    vehicle->SetTransform(&matrix);
    App->player->vehicle->vehicle->getRigidBody()->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
    if (acceleratingChannel != -1)
    {
        App->audio->StopFx(acceleratingChannel);
    }
    
    if (backwardsChannel != -1)
    {
        App->audio->StopFx(backwardsChannel);
    }
}