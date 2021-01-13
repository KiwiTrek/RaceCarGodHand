#include "PhysVehicle3D.h"
#include "Primitive.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// ----------------------------------------------------------------------------
VehicleInfo::~VehicleInfo()
{
	//if(wheels != NULL)
		//delete wheels;
}

// ----------------------------------------------------------------------------
PhysVehicle3D::PhysVehicle3D(btRigidBody* body, btRaycastVehicle* vehicle, const VehicleInfo& info) : PhysBody3D(body), vehicle(vehicle), info(info)
{
}

// ----------------------------------------------------------------------------
PhysVehicle3D::~PhysVehicle3D()
{
	delete vehicle;
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Render()
{
	Cylinder wheel;

	wheel.color = Black;

	for (int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	chassis.color = Red;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());
	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();

	//Left bumper -------------------------------------------
	Cube bumpLeft(info.bump_left_size.x, info.bump_left_size.y, info.bump_left_size.z);
	bumpLeft.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bumpLeft.transform);
	btVector3 offsetBumpLeft(info.bump_left_offset.x, info.bump_left_offset.y, info.bump_left_offset.z);
	offsetBumpLeft = offsetBumpLeft.rotate(q.getAxis(), q.getAngle());
	bumpLeft.transform.M[12] += offsetBumpLeft.getX();
	bumpLeft.transform.M[13] += offsetBumpLeft.getY();
	bumpLeft.transform.M[14] += offsetBumpLeft.getZ();

	Cube jointLeftOne(info.joint_left_one_size.x, info.joint_left_one_size.y, info.joint_left_one_size.z);
	jointLeftOne.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&jointLeftOne.transform);
	btVector3 offsetJointLeftOne(info.joint_left_one_offset.x, info.joint_left_one_offset.y, info.joint_left_one_offset.z);
	offsetJointLeftOne = offsetJointLeftOne.rotate(q.getAxis(), q.getAngle());
	jointLeftOne.transform.M[12] += offsetJointLeftOne.getX();
	jointLeftOne.transform.M[13] += offsetJointLeftOne.getY();
	jointLeftOne.transform.M[14] += offsetJointLeftOne.getZ();

	Cube jointLeftTwo(info.joint_left_two_size.x, info.joint_left_two_size.y, info.joint_left_two_size.z);
	jointLeftTwo.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&jointLeftTwo.transform);
	btVector3 offsetJointLeftTwo(info.joint_left_two_offset.x, info.joint_left_two_offset.y, info.joint_left_two_offset.z);
	offsetJointLeftTwo = offsetJointLeftTwo.rotate(q.getAxis(), q.getAngle());
	jointLeftTwo.transform.M[12] += offsetJointLeftTwo.getX();
	jointLeftTwo.transform.M[13] += offsetJointLeftTwo.getY();
	jointLeftTwo.transform.M[14] += offsetJointLeftTwo.getZ();

	//Right bumper -------------------------------------------
	Cube bumpRight(info.bump_right_size.x, info.bump_right_size.y, info.bump_right_size.z);
	bumpRight.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bumpRight.transform);
	btVector3 offsetBumpRight(info.bump_right_offset.x, info.bump_right_offset.y, info.bump_right_offset.z);
	offsetBumpRight = offsetBumpRight.rotate(q.getAxis(), q.getAngle());
	bumpRight.transform.M[12] += offsetBumpRight.getX();
	bumpRight.transform.M[13] += offsetBumpRight.getY();
	bumpRight.transform.M[14] += offsetBumpRight.getZ();

	Cube jointRightOne(info.joint_right_one_size.x, info.joint_right_one_size.y, info.joint_right_one_size.z);
	jointRightOne.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&jointRightOne.transform);
	btVector3 offsetJointRightOne(info.joint_right_one_offset.x, info.joint_right_one_offset.y, info.joint_right_one_offset.z);
	offsetJointRightOne = offsetJointRightOne.rotate(q.getAxis(), q.getAngle());
	jointRightOne.transform.M[12] += offsetJointRightOne.getX();
	jointRightOne.transform.M[13] += offsetJointRightOne.getY();
	jointRightOne.transform.M[14] += offsetJointRightOne.getZ();

	Cube jointRightTwo(info.joint_right_two_size.x, info.joint_right_two_size.y, info.joint_right_two_size.z);
	jointRightTwo.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&jointRightTwo.transform);
	btVector3 offsetJointRightTwo(info.joint_right_two_offset.x, info.joint_right_two_offset.y, info.joint_right_two_offset.z);
	offsetJointRightTwo = offsetJointRightTwo.rotate(q.getAxis(), q.getAngle());
	jointRightTwo.transform.M[12] += offsetJointRightTwo.getX();
	jointRightTwo.transform.M[13] += offsetJointRightTwo.getY();
	jointRightTwo.transform.M[14] += offsetJointRightTwo.getZ();

	// Front bumper -------------------------------------------
	Cube bumpFront(info.bump_front_size.x, info.bump_front_size.y, info.bump_front_size.z);
	bumpFront.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bumpFront.transform);
	btVector3 offsetBumpFront(info.bump_front_offset.x, info.bump_front_offset.y, info.bump_front_offset.z);
	offsetBumpFront = offsetBumpFront.rotate(q.getAxis(), q.getAngle());
	bumpFront.transform.M[12] += offsetBumpFront.getX();
	bumpFront.transform.M[13] += offsetBumpFront.getY();
	bumpFront.transform.M[14] += offsetBumpFront.getZ();

	Cube bumpFrontLeft(info.bump_front_left_size.x, info.bump_front_left_size.y, info.bump_front_left_size.z);
	bumpFrontLeft.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bumpFrontLeft.transform);
	btVector3 offsetBumpFrontLeft(info.bump_front_left_offset.x, info.bump_front_left_offset.y, info.bump_front_left_offset.z);
	offsetBumpFrontLeft = offsetBumpFrontLeft.rotate(q.getAxis(), q.getAngle());
	bumpFrontLeft.transform.M[12] += offsetBumpFrontLeft.getX();
	bumpFrontLeft.transform.M[13] += offsetBumpFrontLeft.getY();
	bumpFrontLeft.transform.M[14] += offsetBumpFrontLeft.getZ();

	Cube bumpFrontRight(info.bump_front_right_size.x, info.bump_front_right_size.y, info.bump_front_right_size.z);
	bumpFrontRight.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&bumpFrontRight.transform);
	btVector3 offsetBumpFrontRight(info.bump_front_right_offset.x, info.bump_front_right_offset.y, info.bump_front_right_offset.z);
	offsetBumpFrontRight = offsetBumpFrontRight.rotate(q.getAxis(), q.getAngle());
	bumpFrontRight.transform.M[12] += offsetBumpFrontRight.getX();
	bumpFrontRight.transform.M[13] += offsetBumpFrontRight.getY();
	bumpFrontRight.transform.M[14] += offsetBumpFrontRight.getZ();

	Cube jointFrontOne(info.joint_front_one_size.x, info.joint_front_one_size.y, info.joint_front_one_size.z);
	jointFrontOne.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&jointFrontOne.transform);
	btVector3 offsetJointFrontOne(info.joint_front_one_offset.x, info.joint_front_one_offset.y, info.joint_front_one_offset.z);
	offsetJointFrontOne = offsetJointFrontOne.rotate(q.getAxis(), q.getAngle());
	jointFrontOne.transform.M[12] += offsetJointFrontOne.getX();
	jointFrontOne.transform.M[13] += offsetJointFrontOne.getY();
	jointFrontOne.transform.M[14] += offsetJointFrontOne.getZ();

	Cube jointFrontTwo(info.joint_front_two_size.x, info.joint_front_two_size.y, info.joint_front_two_size.z);
	jointFrontTwo.color = White;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&jointFrontTwo.transform);
	btVector3 offsetJointFrontTwo(info.joint_front_two_offset.x, info.joint_front_two_offset.y, info.joint_front_two_offset.z);
	offsetJointFrontTwo = offsetJointFrontTwo.rotate(q.getAxis(), q.getAngle());
	jointFrontTwo.transform.M[12] += offsetJointFrontTwo.getX();
	jointFrontTwo.transform.M[13] += offsetJointFrontTwo.getY();
	jointFrontTwo.transform.M[14] += offsetJointFrontTwo.getZ();

	//Cabin
	Sphere cabin(info.cabin_radious);
	cabin.color = Cyan;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&cabin.transform);
	btVector3 offsetCabin(info.cabin_offset.x, info.cabin_offset.y, info.cabin_offset.z);
	offsetCabin = offsetCabin.rotate(q.getAxis(), q.getAngle());
	cabin.transform.M[12] += offsetCabin.getX();
	cabin.transform.M[13] += offsetCabin.getY();
	cabin.transform.M[14] += offsetCabin.getZ();

	//Motor
	Cube motor(info.motor_size.x, info.motor_size.y, info.motor_size.z);
	motor.color = Gray;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&motor.transform);
	btVector3 offsetMotor(info.motor_offset.x, info.motor_offset.y, info.motor_offset.z);
	offsetMotor = offsetMotor.rotate(q.getAxis(), q.getAngle());
	motor.transform.M[12] += offsetMotor.getX();
	motor.transform.M[13] += offsetMotor.getY();
	motor.transform.M[14] += offsetMotor.getZ();

	Cube motorTubeTopLeft(info.motor_tube_top_left_size.x, info.motor_tube_top_left_size.y, info.motor_tube_top_left_size.z);
	motorTubeTopLeft.color = Black;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&motorTubeTopLeft.transform);
	btVector3 offsetMotorTubeTopLeft(info.motor_tube_top_left_offset.x, info.motor_tube_top_left_offset.y, info.motor_tube_top_left_offset.z);
	offsetMotorTubeTopLeft = offsetMotorTubeTopLeft.rotate(q.getAxis(), q.getAngle());
	motorTubeTopLeft.transform.M[12] += offsetMotorTubeTopLeft.getX();
	motorTubeTopLeft.transform.M[13] += offsetMotorTubeTopLeft.getY();
	motorTubeTopLeft.transform.M[14] += offsetMotorTubeTopLeft.getZ();

	Cube motorTubeTopRight(info.motor_tube_top_right_size.x, info.motor_tube_top_right_size.y, info.motor_tube_top_right_size.z);
	motorTubeTopRight.color = Black;
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&motorTubeTopRight.transform);
	btVector3 offsetMotorTubeTopRight(info.motor_tube_top_right_offset.x, info.motor_tube_top_right_offset.y, info.motor_tube_top_right_offset.z);
	offsetMotorTubeTopRight = offsetMotorTubeTopRight.rotate(q.getAxis(), q.getAngle());
	motorTubeTopRight.transform.M[12] += offsetMotorTubeTopRight.getX();
	motorTubeTopRight.transform.M[13] += offsetMotorTubeTopRight.getY();
	motorTubeTopRight.transform.M[14] += offsetMotorTubeTopRight.getZ();


	chassis.Render();
	bumpLeft.Render();
	bumpRight.Render();
	jointLeftOne.Render();
	jointLeftTwo.Render();
	jointRightOne.Render();
	jointRightTwo.Render();
	bumpFront.Render();
	bumpFrontLeft.Render();
	bumpFrontRight.Render();
	jointFrontOne.Render();
	jointFrontTwo.Render();
	cabin.Render();
	motor.Render();
	motorTubeTopLeft.Render();
	motorTubeTopRight.Render();
}


// ----------------------------------------------------------------------------
void PhysVehicle3D::DebugRender()
{
	Cylinder wheel;

	wheel.color = Blue;

	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		wheel.radius = info.wheels[0].radius;
		wheel.height = info.wheels[0].width;

		vehicle->updateWheelTransform(i);
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(&wheel.transform);

		wheel.Render();
	}

	Cube chassis(info.chassis_size.x, info.chassis_size.y, info.chassis_size.z);
	vehicle->getChassisWorldTransform().getOpenGLMatrix(&chassis.transform);
	btQuaternion q = vehicle->getChassisWorldTransform().getRotation();
	btVector3 offset(info.chassis_offset.x, info.chassis_offset.y, info.chassis_offset.z);
	offset = offset.rotate(q.getAxis(), q.getAngle());

	chassis.transform.M[12] += offset.getX();
	chassis.transform.M[13] += offset.getY();
	chassis.transform.M[14] += offset.getZ();


	chassis.Render();
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::ApplyEngineForce(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].drive == true)
		{
			vehicle->applyEngineForce(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Brake(float force)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].brake == true)
		{
			vehicle->setBrake(force, i);
		}
	}
}

// ----------------------------------------------------------------------------
void PhysVehicle3D::Turn(float degrees)
{
	for(int i = 0; i < vehicle->getNumWheels(); ++i)
	{
		if(info.wheels[i].steering == true)
		{
			vehicle->setSteeringValue(degrees, i);
		}
	}
}

// ----------------------------------------------------------------------------
float PhysVehicle3D::GetKmh() const
{
	return vehicle->getCurrentSpeedKmHour();
}