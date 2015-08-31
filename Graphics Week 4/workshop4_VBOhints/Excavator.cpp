#include "Excavator.h"
#include <windows.h>
#include <math.h>

Excavator::Excavator() : Entity()
{
}

Excavator::Excavator(float locationX,float locationZ, float movementSpeed, 
		float altitudeY,float angleToZAxis, float turretAngle,
		float upperArmAngle, float lowerArmAngle) 
               : Entity(locationX,locationZ,altitudeY,angleToZAxis)
{
	this->movementSpeed = movementSpeed;
	this->turretAngle = turretAngle;
	this->upperArmAngle = upperArmAngle;
	this->lowerArmAngle = lowerArmAngle;
}

Excavator::~Excavator()
{
}

const float Excavator::getTurretAngle()
{
	return turretAngle;
}

const float Excavator::getUpperArmAngle()
{
	return upperArmAngle;
}

const float Excavator::getLowerArmAngle()
{
	return lowerArmAngle;
}




void Excavator::processKeyboardInput()
{
	// left and right turn the Excavator
	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		++angleToZAxisInDegrees;
		if (angleToZAxisInDegrees>=360)
			{
				angleToZAxisInDegrees = 0;
			}
	}
	else 
	{
		if (GetAsyncKeyState(VK_RIGHT) < 0)
		{
			--angleToZAxisInDegrees;
			if (angleToZAxisInDegrees<=0)
			{
				angleToZAxisInDegrees = 360;
			}
			
		}
	}


	// up and down are for moving backwards/forwards
	if (GetAsyncKeyState(VK_UP) < 0)
	{
		locationX -= movementSpeed*sin(pi*(angleToZAxisInDegrees/180.0));
		locationZ -= movementSpeed*cos(pi*(angleToZAxisInDegrees/180.0));
	}
	else 
	{
		if (GetAsyncKeyState(VK_DOWN) < 0)
		{
			locationX += movementSpeed*sin(pi*(float(angleToZAxisInDegrees)/180.0));
			locationZ += movementSpeed*cos(pi*(float(angleToZAxisInDegrees)/180.0));
		}
	}


	// rotate the Excavator turret arround the base
	if (GetAsyncKeyState('Q') < 0)
	{
		--turretAngle;
		if (turretAngle<=0)
		{
			turretAngle = 360;
		}
	}
	else 
	{
		if (GetAsyncKeyState('A') < 0)
		{
			++turretAngle;
			if (turretAngle>=360)
			{
				turretAngle = 0;
			}
		}
	}

	// move upper arm, limit movement between 
	if (GetAsyncKeyState('W') < 0)
	{
		--upperArmAngle;
		if (upperArmAngle<270)
		{
			upperArmAngle = 270;
		}
	}
	else 
	{
		if (GetAsyncKeyState('S') < 0)
		{
			++upperArmAngle;
			if (upperArmAngle>405)
			{
				upperArmAngle = 405;
			}
		}
	}

	// move lower arm
	if (GetAsyncKeyState('E') < 0)
	{
		--lowerArmAngle;
		if (lowerArmAngle<0)
		{
			lowerArmAngle = 0;
		}
	}
	else 
	{
		if (GetAsyncKeyState('D') < 0)
		{
			++lowerArmAngle;
			if (lowerArmAngle>140)
			{
				lowerArmAngle = 140;
			}
		}
	}


}