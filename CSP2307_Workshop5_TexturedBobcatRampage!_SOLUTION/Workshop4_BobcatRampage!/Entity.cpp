// inplementation of Entity class

#include "Entity.h"

Entity::Entity()
{
	locationX = 0;
	locationZ = 0;
	altitudeY = 0;
	angleToZAxisInDegrees = 0;
}

Entity::Entity(float locationX,float locationZ,float altitudeY,float angleToZAxis)
{
	this->locationX = locationX;
	this->locationZ = locationZ;
	this->altitudeY = altitudeY;
	this->angleToZAxisInDegrees = angleToZAxis;
}

Entity::~Entity()
{
}

// getter and setter methods:

const float Entity::getZAngleInDegrees(void)
{
	return angleToZAxisInDegrees;
}

void Entity::setZAngleInDegrees(float angleToZAxisInDegrees)
{
	this->angleToZAxisInDegrees = angleToZAxisInDegrees;
}

void Entity::incrementZAngleInDegrees(float increment)
{
	this->altitudeY += increment;
}

const float Entity::getAltitudeY(void)
{
	return altitudeY;
}
void Entity::setAltitudeY(float altitudeY)
{
	this->altitudeY = altitudeY;
}
void Entity::incrementAltitude(float increment)
{
	this->altitudeY += increment;
}

const float Entity::getLocationX()
{
	return this->locationX;
}

const float Entity::getLocationZ()
{
	return this->locationZ;
}

void Entity::setLocation(float X, float Z)
{
	this->locationX = X;
	this->locationZ = Z;
}

void Entity::incrementLocation(float incrementX, float incrementZ)
{
	this->locationX += incrementX;
	this->locationZ += incrementZ;
}