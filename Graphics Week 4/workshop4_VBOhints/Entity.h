// Entity.h
// encapsulates attributes and methods of things in the world

#ifndef ENTITY
#define ENTITY


class Entity
{
public:
	// construction and destruction
	Entity();
	Entity(float locationX,float locationZ,float altitudeY = 0,float angleToZAxis = 0);
	~Entity();

	// model data
	void setupModel();

	// physics data
	void setupPhysics();

    // getter and setter methods:

	const float getZAngleInDegrees(void);
	void setZAngleInDegrees(float);
	void incrementZAngleInDegrees(float);

	const float getAltitudeY(void);
	void setAltitudeY(float);
	void incrementAltitude(float);

	const float getLocationX();
	const float getLocationZ();
	void setLocation(float X, float Z);
	void incrementLocation(float incrementX, float incrementZ);


protected:
	// entity location in world
	float locationX;
	float locationZ;
	
	// how high above ground is the entity
	float altitudeY;

	// where is the entity facing?
	float angleToZAxisInDegrees;

	// physics data....

	// geometry and vertex data....
};

#endif