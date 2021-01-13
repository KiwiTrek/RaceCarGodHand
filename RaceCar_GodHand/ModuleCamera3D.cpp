#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include <cmath>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	cameraType = CameraType::EAGLE;

	Position = vec3(0.0f, 5.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
	playerDistance = 15.0f;
	playerVerticalAngle = 20.0f;
	//playerHorizontalAngle = 0.0;
    lookBack = false;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	cameraType = CameraType::EAGLE;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		switch (cameraType)
		{
		case CameraType::NORMAL:
			cameraType = CameraType::DEBUG;
			break;
		case CameraType::EAGLE:
			cameraType = CameraType::DEBUG;
			break;
		case CameraType::DEBUG:
			if (App->scene_intro->start)
			{
				cameraType = CameraType::EAGLE;
			}
			else
			{
				cameraType = CameraType::NORMAL;
				Position = { App->player->GetX(), App->player->GetY() + 10.0f, App->player->GetZ() + 15.0f };
			}
			break;
		}
	}

	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	//LOG("%f %f %f", Position.x, Position.y, Position.z);
	vec3 newPos(0, 0, 0);
    lookBack = false;
	switch (cameraType)
	{
		case CameraType::NORMAL:
		{
			if (!App->player->victory && !App->player->lose)
			{
				float zoomSpeed = 15.0f * dt;
				if (App->input->GetMouseZ() > 0) playerDistance -= zoomSpeed;
				if (App->input->GetMouseZ() < 0) playerDistance += zoomSpeed;
				if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
				{
					playerVerticalAngle = playerVerticalAngle - (App->input->GetMouseYMotion() * 0.25f);
					//playerHorizontalAngle = playerHorizontalAngle - (App->input->GetMouseYMotion() * 0.25f);
				}
				if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
				{
					lookBack = true;
				}

				FollowPlayer(lookBack);
			}
			break;
		}
		case CameraType::DEBUG:
		{
			float speed = 10.0f * dt;
			if (App->input->GetKey(SDL_SCANCODE_RSHIFT) == KEY_REPEAT)
				speed = 50.0f * dt;

			if (App->input->GetKey(SDL_SCANCODE_COMMA) == KEY_REPEAT) newPos.y += speed;
			if (App->input->GetKey(SDL_SCANCODE_PERIOD) == KEY_REPEAT) newPos.y -= speed;

			if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) newPos += Z * speed;

			if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) newPos += X * speed;

			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				float Sensitivity = 0.25f;

				Position -= Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * Sensitivity;

					X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
					Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * Sensitivity;

					Y = rotate(Y, DeltaY, X);
					Z = rotate(Z, DeltaY, X);

					if (Y.y < 0.0f)
					{
						Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						Y = cross(Z, X);
					}
				}

				Position = Reference + Z * length(Position);
			}

			break;
		}
		case CameraType::EAGLE:
		{
			Position = { 0,150,200 };
			LookAt({ 0,0,0 });
			break;
		}
	}

	Position += newPos;
	Reference += newPos;

	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::FollowPlayer(bool lookBack)
{
	// Move position of camera reference frame to player's CM
	Reference = { (float)App->player->GetX(), (float)App->player->GetY(), (float)App->player->GetZ() };

	// Rotate unitary axis XYZ of the camera's reference frame to align with local car orientation
	btQuaternion newQuat = App->player->GetRotationQuaternion();											//Obtain rotation of car in quaternion form
	vec3 newEulerAngles = ToEulerAngles(newQuat);															//Transform quaternion to corresponding Euler angles
	//LOG("%f %f %f", RADTODEG * newEulerAngles.x, RADTODEG * newEulerAngles.y, RADTODEG * newEulerAngles.z);
	newEulerAngles.x *= RADTODEG;																			//Convert to degrees because rotate function needs degrees :)
	newEulerAngles.y *= RADTODEG;
	newEulerAngles.z *= RADTODEG;
	X = rotate(vec3(1.0f, 0.0f, 0.0f), newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));							//Rotate unitary vector x from reference frame along the Y axis in world scenario
	Y = rotate(vec3(0.0f, 1.0f, 0.0f), newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));							//Rotate unitary vector y from reference frame along the Y axis in world scenario
	Z = rotate(vec3(0.0f, 0.0f, 1.0f), newEulerAngles.y, vec3(0.0f, 1.0f, 0.0f));							//Rotate unitary vector z from reference frame along the Y axis in world scenario
	// WE ONLY ROTATE ALONG THE Y axis (Vertical axis) so that the camera remains horizontal at this point

	// Rotate camera, since car orientation is reversed
    if (lookBack == false)
    {
        X = rotate(X, 180.0f, vec3(0.0f, 1.0f, 0.0f));
        Y = rotate(Y, 180.0f, vec3(0.0f, 1.0f, 0.0f));
        Z = rotate(Z, 180.0f, vec3(0.0f, 1.0f, 0.0f));
    }

	//// Rotate camera following Y axis (User Input)
	//X = rotate(X, playerHorizontalAngle, vec3(0.0f, 1.0f, 0.0f));
	//Y = rotate(Y, playerHorizontalAngle, vec3(0.0f, 1.0f, 0.0f));
	//Z = rotate(Z, playerHorizontalAngle, vec3(0.0f, 1.0f, 0.0f));

	// Raise camera following X axis
	Y = rotate(Y, -playerVerticalAngle, X);
	Z = rotate(Z, -playerVerticalAngle, X);
	if (Y.y < 0.0f)
	{
		Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
		Y = cross(Z, X);
	}

	// Calculate camera position
	Position = Reference + Z * playerDistance;			// Move away the camera a preset distance along the Z axis

	// Change camera accordingly
	CalculateViewMatrix();
}

vec3 ModuleCamera3D::ToEulerAngles(btQuaternion q) {
	vec3 angles;

	// roll (x-axis rotation)
	float sinr_cosp = 2 * (q.getW() * q.getX() + q.getZ() * q.getY());
	float cosr_cosp = 1 - 2 * (q.getX() * q.getX() + q.getZ() * q.getZ());
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// pitch (z-axis rotation)
	float sinp = 2 * (q.getW() * q.getZ() - q.getY() * q.getX());
	if (std::abs(sinp) >= 1)
		angles.z = std::copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		angles.z = std::asin(sinp);

	// yaw (y-axis rotation)
	float siny_cosp = 2 * (q.getW() * q.getY() + q.getX() * q.getZ());
	float cosy_cosp = 1 - 2 * (q.getZ() * q.getZ() + q.getY() * q.getY());
	angles.y = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}