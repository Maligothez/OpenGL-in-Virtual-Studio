#pragma once

class Vertex3
{
private:
    float m_cartesian[3];// m_ is used to indicate 'member variable'
public:
	// the following overload means that whenever we refer to the vector, without indicating a method,
	// the cartesian member variables are returned.  This makes the object easy to use with OpenGL
	// if you want the vector to represent a vertex just call glVertex3fv(myVector);
    // In the method, the first 'const' means that the thing that is returned (an array of floats)
	// cant be changed by whoever requests it.  The second 'const' means that this method does not
	// modify the vector3f object - compiler will catch an attempt to change const's at compile time
	// using consts does not add any overhead at run time.
	operator const float*() const
	{
		return m_cartesian;  
	}


	// constructor - this is called when every new object is created
	Vertex3(float x = 0.0, float y = 0.0, float z = 0.0)
	{
		set(x,y,z);
	}

	// setter method to change the cartesian X, Y and Z components all in one go
	void set(float newX, float newY, float newZ)
	{
		// for definition of methods x(), y(), and z() see 
		x() = newX;
		y() = newY;
		z() = newZ;
	}

	// these are the public getter and setter methods to retreive the vectors cartesian 
	// coordinates they return a reference, meaning what is returned can also be used to
	// change the object (see their use in the set method above)
	float& x(){return m_cartesian[0];}
	float& y(){return m_cartesian[1];}
	float& z() {return m_cartesian[2];}

	// define constant versions of the getter methods (value returned cant be used to change object)
	float x() const {return m_cartesian[0];}
	float y() const {return m_cartesian[1];}
	float z() const {return m_cartesian[2];}

	// overload the = operator so we can set one vector to another one
	// declaring the input vector as 'const' indicates we wont change it
	void operator=(const Vertex3 other)
	{
		set(other.x(), other.y(), other.z());
	}
};