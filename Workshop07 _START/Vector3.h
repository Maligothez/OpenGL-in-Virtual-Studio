#ifndef VECTOR_3
#define VECTOR_3

#include <cmath> // to enable use of math functions (eg. std::sqrt)

// class Vector3f - 3D vector class holding floating point numbers
// If you have done the C++ unit - perhaps you would like to template
// this to take any type.

// Some of these ideas came from code by Marijn Haverbeke - available from the
// Game Programming Wiki at (http://gpwiki.org/index.php/C:Vectors)
class Vector3f
{
private:
	// m_cartesian[3] these are the actual cartesian coordinates of the vector
	// stored as an array of length 3.  Declared private - this can only be seen
	// or changed through the public setter and getter methods.
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
	Vector3f(float x = 0.0, float y = 0.0, float z = 0.0)
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
	void operator=(const Vector3f other)
	{
		set(other.x(), other.y(), other.z());
	}

	// function to get the magnitude of the vector
	float length() const
	{
		return std::sqrt(lengthSquared());
	}

	// when comparing the magnitude of two vectors to see which is greater, its enough
	// to compare squared magnitudes and save doing the square root computation.
	float lengthSquared() const
	{
		return x()*x() + y()*y() + z()*z();
		
	}

	// adding another vector to 'this' vector - overload the += operator in a member method
	void operator +=(const Vector3f& other)
	{
		set(x()+other.x(), y()+other.y(), z()+other.z());
	}

}; // this is the end of the class definition - its member variables and methods

// non-member methods (functions) can also be defined, these are more intuitive for some operators

// For example, adding two vectors to produce a third vector is handled better by a non-member function
// here using the overloaded + operator.  Declaring this function 'inline' suggests to the compiler 
// to paste this code into wherever the function is called, rather than jump here every time.
// The compiler does not have to obey the 'inline' keyword.
inline const Vector3f operator+(const Vector3f& one, const Vector3f& two)
{
	// vector addition
	return Vector3f(one.x() + two.x(), one.y() + two.y(), one.z() + two.z());
}

inline const Vector3f operator-(const Vector3f& one, const Vector3f& two)
{
	// vector sibtraction
	return Vector3f(one.x() - two.x(), one.y() - two.y(), one.z() - two.z());
}

inline const Vector3f operator*(const float& c, const Vector3f& two)
{
	// vector scaling
	return Vector3f(c * two.x(), c* two.y(), c* two.z());
}

inline const float dotProd(const Vector3f& one, const Vector3f& two)
{
	// vector dot product
	return (one.x() * two.x() + one.y() * two.y() + one.z() * two.z());
}

inline const Vector3f crossProd(const Vector3f& one, const Vector3f& two)
{
	// vector cross product
	return Vector3f(one.y()*two.z() - one.z()*two.y(), one.z()*two.x() - one.x()*two.z(), one.x()*two.y() - one.y()*two.x());
}


#endif