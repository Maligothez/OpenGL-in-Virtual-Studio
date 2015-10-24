#pragma once
#ifndef BULLET
#define BULLET

#include "Geometry.h"
#include "Entity.h"
#include "Vector3.h"
#include <vector>

using namespace std;

const double pi = 3.1415927;

class Bullet : public Entity
{
public:
	Bullet();
	Bullet(float locationX, float locationZ, float movementSpeed = 0.5,
		float altitudeY = 0, float angleToZAxis = 0);
	~Bullet();
	
	const float getSpeed();

	bool processKeyboardInput(); //vector<staticGameObject> &objects);
	bool checkCollisions(vector<Geometry> &things);
private:
	float movementSpeed;
	float currentSpeedX;
	float currentSpeedZ;
	float turningSpeed;
	float currentTurningSpeed;
	float friction;
	float turretAngle;
	float upperArmAngle;
	float lowerArmAngle;
	bool fpsMode;
};


