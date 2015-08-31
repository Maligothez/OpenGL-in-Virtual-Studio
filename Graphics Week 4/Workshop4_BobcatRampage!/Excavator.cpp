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

	friction = 0.95f;
	turningSpeed = movementSpeed;
	currentTurningSpeed = 0;
	currentSpeedX = 0;
	currentSpeedZ = 0;
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


bool Excavator::checkCollisions(vector<Geometry> &things)
{
	return true;
}


bool Excavator::processKeyboardInput()
{	
	bool gameWon = 0;
	// left and right turn the Excavator
	if (GetAsyncKeyState(VK_LEFT) < 0)
	{
		currentTurningSpeed += 2*turningSpeed;
	}
	else 
	{
		if (GetAsyncKeyState(VK_RIGHT) < 0)
		{
			currentTurningSpeed -= 2*turningSpeed;			
		}
	}
	angleToZAxisInDegrees += currentTurningSpeed;
	currentTurningSpeed *= friction;
	if (angleToZAxisInDegrees>=360)
	{
		angleToZAxisInDegrees -=360;
	}
	if (angleToZAxisInDegrees<=0)
	{
		angleToZAxisInDegrees += 360;
	}
	// up and down are for moving backwards/forwards

	Vector3f oldPosition = Vector3f(locationX, altitudeY, locationZ);

	bool moved = FALSE;
	if (GetAsyncKeyState(VK_UP) < 0)
	{
		//locationX -= movementSpeed*(float)sin(pi*(angleToZAxisInDegrees/180.0));
		//locationZ -= movementSpeed*(float)cos(pi*(angleToZAxisInDegrees/180.0));

		currentSpeedX -= movementSpeed*(float)sin(pi*(angleToZAxisInDegrees/180.0));
		currentSpeedZ -= movementSpeed*(float)cos(pi*(angleToZAxisInDegrees/180.0));

		moved = TRUE;
	}
	else 
	{
		if (GetAsyncKeyState(VK_DOWN) < 0)
		{
			//locationX += movementSpeed*(float)sin(pi*(float(angleToZAxisInDegrees)/180.0));
			//locationZ += movementSpeed*(float)cos(pi*(float(angleToZAxisInDegrees)/180.0));

			currentSpeedX += movementSpeed*(float)sin(pi*(float(angleToZAxisInDegrees)/180.0));
			currentSpeedZ += movementSpeed*(float)cos(pi*(float(angleToZAxisInDegrees)/180.0));

			moved = TRUE;
		}
	}

	locationX += currentSpeedX;
	locationZ += currentSpeedZ;
	
	currentSpeedX *= friction;
	currentSpeedZ *= friction;
	
	if (moved)
	{
		Vector3f nextPosition = Vector3f(locationX, altitudeY, locationZ);
		// check for collisiosn with the static objects
		//for (vector<staticGameObject>::iterator object = objects.begin(); object!=objects.end(); ++object)
		//{
		//	if (object->isColiding(nextPosition))
		//	{
		//		// if its the exams - pick them up
		//		if (object->type()==EXAMS)
		//		{
		//			// erase the exams object from the stl vector
		//			// and update the iterator (will set it to the next object)
		//			object = objects.erase(object);
		//			// we dont want the next object yet (the loop does the incrementing)
		//			// so decrement the iterator
		//			object--;

		//			gameWon = 1; // set game won to true
		//		}
		//		else
		//		{
		//		// reset to previous position
		//		locationX = oldPosition.x();
		//		altitudeY = oldPosition.y();
		//		locationZ = oldPosition.z();
		//		}
		//	}
		//}
		return gameWon;
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
		if (upperArmAngle<0)
		{
			upperArmAngle = 360;
		}
	}
	else 
	{
		if (GetAsyncKeyState('S') < 0)
		{
			++upperArmAngle;
			if (upperArmAngle>360)
			{
				upperArmAngle = 0;
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
			if (lowerArmAngle>360)
			{
				lowerArmAngle = 360;
			}
		}
	}

	return gameWon;

}