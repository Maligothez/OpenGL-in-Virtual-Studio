#pragma once
#ifndef EXCAVATOR
#define EXCAVATOR

#include "Geometry.h"
#include "Entity.h"
#include "Vector3.h"
#include <vector>

using namespace std;

const double pi=3.1415927;

class Excavator: public Entity
{
public:
	Excavator();
	Excavator(float locationX,float locationZ, float movementSpeed = 0.1, 
		float altitudeY = 0,float angleToZAxis=0, float turrentAngle=180,
		float upperArmAngle=315, float lowerArmAngle=90);
	~Excavator();
	const float getTurretAngle();
	const float getUpperArmAngle();
	const float getLowerArmAngle();
	const bool getFPSMode();
	const float getSpeed();

	bool processKeyboardInput(vector<Geometry> &worldThings); //vector<staticGameObject> &objects);
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
	float health;
};



#endif
