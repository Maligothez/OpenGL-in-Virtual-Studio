#pragma once

// game logic for the bobcat game
// Created September 2009, 

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
const float HOUSERANGE = 40;
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

			
			Vector3f sheepAverage(0,0,0);
			if (!current->getName().compare("sheep")) // if the object is a sheep
			{
				Vector3f heading(0,0,0); // this is the vector that must be set


				
				//*****************************************************//
				// Y O U R   C O D E   S T A R T S   H E R E
				//****************************************************//
				
				

				Vector3f sheepPushVector(0.0,0,0.0);
				Vector3f sheepFlockVector(0,0,0);
				Vector3f sheepCohesionForce(0,0,0);
				Vector3f alignmentVector(0,0,0);
				Vector3f excavatorVector(0,0,0);
				Vector3f houseVector(0,0,0);

				Vector3f sheepTotal(0,0,0);
				Vector3f sheepAverage(0,0,0);
				int averageCount = 0;
				
				
					// 
				// get a vector pointing from the sheep towards other sheep
				//if (!current->getName().compare("sheep"))
				//{
				for(vector<Geometry>::iterator j = gameObjects.begin(); j<gameObjects.end(); j++) {

					if (!j->getName().compare("sheep")&& current->getID() != j->getID()) {

						sheepFlockVector = current->getPosition() - j->getPosition();

						//cohesion, aquiring sheep total and finding average
						if (j->isColiding(current->getPosition(), (current->getBoundingSphereRadius())+FLOCKRADIUS))
						{
							
							sheepTotal = sheepTotal + j->getPosition();
							averageCount++;
							
							alignmentVector += j->getAngle();
												

						}	

						//seperation
						sheepPushVector = current->getPosition() - j->getPosition(); 
						if (current->isColiding(j->getPosition(), j->getBoundingSphereRadius()))
						{

							float sheepPushForceLength = sheepPushVector.length();		
							if (sheepPushForceLength != 0) {
								sheepPushVector = (1 / sheepPushForceLength)*sheepPushVector; // normalize to get direction only
							}
							// scale force so its greater closer to the sheep
							float pushMaxForceLength = 50;
							sheepPushForceLength = max(0, (pushMaxForceLength-sheepPushForceLength));
							sheepPushVector = sheepPushForceLength*sheepPushVector;

							heading += sheepPushVector; 
						}	



					}

					
					//}

					if (!j->getName().compare("excavator"))
					{

						


							excavatorVector = current->getPosition() - j->getPosition();

							if (j->isColiding(current->getPosition(), current->getBoundingSphereRadius()+BOBCATRANGE))
							{
								float excavatorLength = excavatorVector.length();			
								
								excavatorVector = (1/excavatorLength)*excavatorVector; // normalize to get direction only


								float excavatorMaxForceLength = 250;
								excavatorLength = max(0, (excavatorMaxForceLength-excavatorLength));
								excavatorVector = excavatorLength*excavatorVector;

								heading += excavatorVector;
							}	


						

					}

					if (!j->getName().compare("house"))
					{

						


							houseVector = j->getPosition() - current->getPosition();

							if (j->isColiding(current->getPosition(), (current->getBoundingSphereRadius())+HOUSERANGE))
							{
								float houseLength = houseVector.length();							
								houseVector = (1/houseLength)*houseVector; // normalize to get direction only


								float houseMaxForceLength = 100;
								houseLength = max(0, (houseMaxForceLength-houseLength));
								sheepFlockVector = houseLength*houseVector;


							}	



						

					}

					//heading += sheepFlockVector; 
					
					
				}

				//cohesion force. Getting average of posiitons and using it as centre
				alignmentVector = alignmentVector - current->getPosition();
				float alignmentVectorLength =	alignmentVector.length();
				alignmentVector = (1/alignmentVectorLength) * alignmentVector;
				float alignmentMaxForceLength = 50;
						alignmentVectorLength = max(0, (alignmentMaxForceLength-alignmentVectorLength));
							alignmentVector = alignmentVectorLength*alignmentVector;
									
				sheepAverage = Vector3f(sheepTotal.x()/averageCount, sheepTotal.y()/averageCount, sheepTotal.z()/averageCount);

				sheepCohesionForce = sheepAverage - current->getPosition();
				float sheepCohesionForceLength = sheepCohesionForce.length();		
				if (sheepCohesionForceLength != 0) {
					sheepCohesionForce = (1 / sheepCohesionForceLength)*sheepCohesionForce; // normalize to get direction only
				}
							// scale force so its greater closer to the sheep
							float cohesionMaxForceLength = 50;
							sheepCohesionForceLength = max(0, (cohesionMaxForceLength-sheepCohesionForceLength));
							sheepCohesionForce = sheepCohesionForceLength*sheepCohesionForce;
							
							heading += alignmentVector;
							heading+= sheepCohesionForce;
				// Sample code to move the sheep diagonally down - delete and replace with your code
				//Vector3f sampleVector(0.1f,0.0f,0.1f);

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