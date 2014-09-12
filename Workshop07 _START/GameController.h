#pragma once

// game logic for the bobcat game
// Created September 2009, Martin Masek, SCSS - Edith Cowan University

#include<vector>
#include<float.h>



// game logic constants
const float FLOCKRADIUS = 40;
const float LEFTBOUNDARY = -200+5;
const float RIGHTBOUNDARY = 200-5;
const float TOPBOUNDARY = -60+10;
const float BOTTOMBOUNDARY = 60-10;

const float BOBCATRANGE = 40;
const float WOLFRANGE = 50;
const float HOUSERANGE = 20;
const float BOUNDARYRANGE = 5;

int sheepHome = 0;




class GameController
{
public:
	static void runGameLogic(vector<Geometry> &gameObjects)
	{
		bool deleted = collisionCheck(gameObjects);

		updateSheep(gameObjects);

		if (deleted)
		{
			purgeDeletedObjects(gameObjects);
		}

	
	}

private:
	static bool collisionCheck(vector<Geometry> &gameObjects)
	{	
		bool deleted = false;
		// check collisions between excavator and other objects
		for(vector<Geometry>::iterator i = gameObjects.begin(); i<gameObjects.end(); i++)
		{
			if (!i->getName().compare("sheep"))
			{
			// check boundary collisions
			Vector3f currentPosition= i->getPosition();
			if (currentPosition.z()>BOTTOMBOUNDARY)
			{
				currentPosition.set(currentPosition.x(),currentPosition.y(),BOTTOMBOUNDARY);
			}
			if (currentPosition.z()<TOPBOUNDARY)
			{
				currentPosition.set(currentPosition.x(),currentPosition.y(),TOPBOUNDARY);
			}

			if (currentPosition.x()>RIGHTBOUNDARY)
			{
				currentPosition.set(RIGHTBOUNDARY,currentPosition.y(),currentPosition.z());
			}
			if (currentPosition.x()<LEFTBOUNDARY)
			{
				currentPosition.set(LEFTBOUNDARY,currentPosition.y(),currentPosition.z());
			}
			i->setPosition(currentPosition,i->getName());
			}


			// check sheep collisions
			if (!i->getName().compare("sheep"))
			{
				// if its a house - the sheep is home
				for(vector<Geometry>::iterator j = gameObjects.begin(); j<gameObjects.end(); j++)
				{
					if (!j->getName().compare("house"))
					{
						if (j->isColiding(i->getPosition(),i->getBoundingSphereRadius()))
						{
							// mark the sheep for deletion
							i->setName((string)"delete");
							sheepHome++;
							deleted = true;
						}
					}
				}

			}




			if (!i->getName().compare("excavator"))
			{
				// check collisions between the excavator and other things in the world
				for(vector<Geometry>::iterator j = gameObjects.begin(); j<gameObjects.end(); j++)
				{
					if (!j->getName().compare("house"))
					{
						if (j->isColiding(i->getPosition(),i->getBoundingSphereRadius()))
						{
							// clear VBO
							//if (j->hasVBO())
							//{
							//	j->clearVBO();
							//}
							j->loadGeometry("resources\\houseRubble.ASE");
							j->setName((string)"rubble");
						}
					}
					else if (!j->getName().compare("sheep"))
					{
						if (j->isColiding(i->getPosition(),i->getBoundingSphereRadius()))
						{
							// change the excavator colour to red if caught!
							i->setColour(1,0,0);
						}
					}
				}
			}
		}
		return deleted;
	}


	static void purgeDeletedObjects(vector<Geometry> &gameObjects)
	{
		// iterate through and remove objects marked for deletion
		for (vector<Geometry>::iterator current = gameObjects.begin(); current<gameObjects.end(); current++)
		{
			if (!current->getName().compare("delete"))
			{
				current = gameObjects.erase(current);
				current--;

				// **** for some reason, the above invalidates the VBO of the last
				// **** element in the vector - so need to re-create it!
				int vecSize = gameObjects.size();
				gameObjects[vecSize-1].createOpenGLVertexBufferObject();
				
			}
		}
	}


	static void updateSheep(vector<Geometry> &gameObjects)
	{
		// this method updates sheep position and orientation based on flocking rules


		// go through the game objects and proces each sheep
		for(vector<Geometry>::iterator current = gameObjects.begin(); current<gameObjects.end(); current++)
		{
			if (!current->getName().compare("sheep")) // if the object is a sheep
			{
				Vector3f heading(0,0,0); // this is the vector that must be set

				//*****************************************************//
				// Y O U R   C O D E   S T A R T S   H E R E
				//****************************************************//
				


				// Sample code to move the sheep diagonally down - delete and replace with your code
				Vector3f sampleVector(0.1f,0.0f,0.1f);
				heading += sampleVector;
				// end of sample code




				//*************************************************************
				//   Y O U R   C O D E   E N D S   H E R E
				//*************************************************************


				//
				// At this point we have have a force vector that acts on the sheep
				// this tells us
				//   a) the direction the sheep should be facing in
				//   b) (a measure of) how fast it should move in that direction
				//
				// we rotate the sheep in the right direction
				// by some increment, and move it forward.
				//


				// current position of the sheep
				Vector3f newPosition = current->getPosition();

				// current orientation of he sheep (only interested in rotation around y-axis)
				Vector3f orientation = current->getAngle(); 
				float angleAroundY = orientation.y();


				// get desired heading angle
				if (heading.length()!=0)
				{
				heading = (1/heading.length())*heading; // normalize heading
				}


				float desiredAngleAroundY = (180/(float)pi)*acos(heading.x()); // get the angle in degrees (acos works in radians)


				// adjust for the quadrant we are in (acos is only good between 0 and 180 degrees)				
				if (heading.z()>0)
				{
					desiredAngleAroundY = 360 - desiredAngleAroundY;
				}

				// ************
				// turn the sheep
				// ************

				// work out distance to turn (in degrees)
				float turningDirection = desiredAngleAroundY - angleAroundY;

				// see if its a shorter to turn the other way
				if (turningDirection>180)
				{
					turningDirection = turningDirection - 360;
				}
				if (turningDirection<-180)
				{
					turningDirection = turningDirection + 360;
				}

				// set the new angle based on a turning speed
				float turningSpeed = 0.1f;
				angleAroundY += turningSpeed*turningDirection;
				orientation.set(orientation.x(), angleAroundY, orientation.z());
				current->setAngle(orientation, (string)"sheep");

				// ********************
				// move the sheep forward
				// ********************

				float movementSpeed = 1;
				float xIncrement = movementSpeed*(float)sin(pi*((-90+angleAroundY)/180.0));
				float zIncrement = movementSpeed*(float)cos(pi*((-90+angleAroundY)/180.0));
				heading.set(-xIncrement,0,-zIncrement);
				newPosition += heading;
				current->setPosition(newPosition,(string)"sheep");
				if (_isnan(newPosition.x()))
				{
					float aa = newPosition.x();

				}
			}
		}
	}

};