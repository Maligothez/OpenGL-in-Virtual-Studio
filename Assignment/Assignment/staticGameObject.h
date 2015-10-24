#ifndef STATIC_OBJECT
#define STATIC_OBJECT

#include "Vector3.h"
#include "Geometry.h"

const int HOUSE = 0;
const int EXAMS = 1;

class staticGameObject
{
private:
	int m_objectType;
	Vector3f m_objectPosition;
	Vector3f m_objectOrientation;
	float m_boundSphereRadius;

public:
	staticGameObject(){};
	staticGameObject(int type, Vector3f position, float bounds)
	{
		m_objectType = type;
		m_objectPosition = position;
		m_boundSphereRadius = bounds;
	}
	~staticGameObject(){};

	// getters and setters

	int type()
	{
		return m_objectType;
	}
	
	void type(int type)
	{
		m_objectType = type;
	}

	Vector3f position()
	{
		return m_objectPosition;
	}

	void position(Vector3f position)
	{
		m_objectPosition = position;
	}

	float bounds()
	{
		return m_boundSphereRadius;
	}

	void bounds(float bounds)
	{
		m_boundSphereRadius = bounds;
	}

	bool isColiding(Vector3f position)
	{
		// bounding sphere collision detection, assuming the player sphere radius is the
		// same as the coliding objects - thats a big assumption though!
		Vector3f displacement = position - m_objectPosition;
		if (displacement.lengthSquared()<(2*m_boundSphereRadius*m_boundSphereRadius))
		{
			return (1);
		}
		else
		{
			return (0);
		}
	}



};


#endif