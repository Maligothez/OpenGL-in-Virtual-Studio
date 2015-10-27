#include "Excavator.h"
#include <windows.h>
#include <math.h>
#include "Geometry.h"

#pragma comment(lib, "Winmm.lib")
#include <Mmsystem.h>


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

	friction = 0.9f;
	turningSpeed = movementSpeed;
	currentTurningSpeed = 0;
	currentSpeedX = 0;
	currentSpeedZ = 0;
	fpsMode = false;
}

Excavator::~Excavator()
{
}

const float Excavator::getTurretAngle()
{
	return turretAngle;
}

const float Excavator::getSpeed()
{
	return sqrt(pow(currentSpeedX,2) + pow(currentSpeedZ,2));
}

const float Excavator::getUpperArmAngle()
{
	return upperArmAngle;
}

const float Excavator::getLowerArmAngle()
{
	return lowerArmAngle;
}

const bool Excavator::getFPSMode()
{
	return fpsMode;
}

bool Excavator::checkCollisions(vector<Geometry> &things)
{
	return true;
}

void bulletShoot(float locationX, float altitudeY, float locationZ, vector<Geometry> &worldThings)
{
	Geometry bullet;
	bullet.setName((string)"bullet");
	bullet.loadGeometry("resources\\sheep\\sheep.ASE");
	bullet.setColour(1.0f, 1.0f, 1.0f);
	bullet.setPosition(Vector3f(locationX, altitudeY, locationZ), "bullet");
	worldThings.push_back(bullet);

	Vector3f heading(0, 0, 0); // this is the vector that must be set

	Vector3f newPosition = bullet.getPosition();

	// current orientation of he sheep (only interested in rotation around y-axis)
	//Vector3f orientation = bullet.getAngle();
	//float angleAroundY = orientation.y();
	//if (heading.length() != 0)
//	{
	//	heading = (1 / heading.length())*heading; // normalize heading
	//}
	
	//float desiredAngleAroundY = (180 / (float)pi)*acos(heading.x()); // get the angle in degrees (acos works in radians)
	
	// adjust for the quadrant we are in (acos is only good between 0 and 180 degrees)				
	//if (heading.z() > 0)
	//{
		//desiredAngleAroundY = 360 - desiredAngleAroundY;
	//}
	//float turningDirection = desiredAngleAroundY - angleAroundY;

	//if (turningDirection > 180)
	//{
		//turningDirection = turningDirection - 360;
	//}
	//if (turningDirection < -180)
	//{
		//turningDirection = turningDirection + 360;
	//}

	// set the new angle based on a turning speed
	//float turningSpeed = 0.1f;
	//angleAroundY += turningSpeed*turningDirection;
	//orientation.set(orientation.x(), angleAroundY, orientation.z());
	//bullet.setAngle(orientation, (string)"bullet");

	// ********************
	// move the sheep forward
	// ********************

	//float movementSpeed = 1;
	//float xIncrement = movementSpeed*(float)sin(pi*((-90 + angleAroundY) / 180.0));
	//float zIncrement = movementSpeed*(float)cos(pi*((-90 + angleAroundY) / 180.0));
	//heading.set(-xIncrement, 0, -zIncrement);
	//newPosition += heading;
	//bullet.setPosition(newPosition, (string)"bullet");
	//if (_isnan(newPosition.x()))
	//{
		//float aa = newPosition.x();

	//}
}


bool Excavator::processKeyboardInput(vector<Geometry> &worldThings)
{	
	bool gameWon = 0;
	static float oldTime = (float)timeGetTime();
	if (GetKeyState(VK_TAB) < 0)
	{
		float newTime = (float)timeGetTime();
		if ((newTime-oldTime)>100) // check if more than 100 miliseconds have passed
		{
			fpsMode = !fpsMode;
			oldTime = newTime;
		}
	}
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


	if (locationZ>60 && currentSpeedZ>0)
	{
		locationZ = 60;
		currentSpeedZ = -currentSpeedZ;
	}
	if (locationZ<-60 && currentSpeedZ<0)
	{
		locationZ=-60;
		currentSpeedZ = -currentSpeedZ;
	}
	
	if (locationX>200 && currentSpeedX>0)
	{
		locationX = 200;
		currentSpeedX = -currentSpeedX;
	}
	if (locationX<-200 && currentSpeedX<0)
	{
		locationX=-200;
		currentSpeedX = -currentSpeedX;
	}
	
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
	if (GetAsyncKeyState(VK_LCONTROL) < 0)
	{

		bulletShoot(locationX, altitudeY, locationZ, worldThings);

	}

	return gameWon;

}



