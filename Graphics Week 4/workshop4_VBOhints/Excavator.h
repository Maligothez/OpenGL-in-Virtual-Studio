#include "Entity.h"

#ifndef EXCAVATOR

#define EXCAVATOR


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

	void processKeyboardInput();
private:
	float movementSpeed;
	float turretAngle;
	float upperArmAngle;
	float lowerArmAngle;
};



#endif
