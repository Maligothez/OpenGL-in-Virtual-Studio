#pragma once

#include "Vector3.h"
#include <cmath> // to enable use of math functions (eg. std::sqrt)

const float PI = 3.1416f;
const float DEG_TO_RAD = PI/180;

// class Matrix4x4f - 4x4 matrix class holding floating point numbers
// built with matrix operations relevant to transformation of real-time
// graphics.
//
// Built to work with the Vector3f vector class
//
// Author: Martin Masek - Edith Cowan University 4th July 2007.

class Matrix4x4f
{
private:
	// m_matrixElement[16] - the actual matrix (4x4 = 16 elements).
	// element numbers shown (maybe):
	//                    _           _
	//                   | 00 01 02 03 |
	// m_matrixElement = | 10 11 12 13 |
	//                   | 20 21 22 23 |
	//                   |_30 31 32 33_|
	//                              

	float m_matrixElement[16];

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
		return m_matrixElement;  
	}

	// these are the public getter and setter methods to retreive the vectors cartesian 
	// coordinates they return a reference, meaning what is returned can also be used to
	// change the object (see their use in the set method above)

public:
	float& m00() {return m_matrixElement[0];}
	float& m01() {return m_matrixElement[1];}
	float& m02() {return m_matrixElement[2];}
	float& m03() {return m_matrixElement[3];}
	float& m10() {return m_matrixElement[4];}
	float& m11() {return m_matrixElement[5];}
	float& m12() {return m_matrixElement[6];}
	float& m13() {return m_matrixElement[7];}
	float& m20() {return m_matrixElement[8];}
	float& m21() {return m_matrixElement[9];}
	float& m22() {return m_matrixElement[10];}
	float& m23() {return m_matrixElement[11];}
	float& m30() {return m_matrixElement[12];}
	float& m31() {return m_matrixElement[13];}
	float& m32() {return m_matrixElement[14];}
	float& m33() {return m_matrixElement[15];}

public:
	// define constant versions of the getter methods (value returned cant be used to change object)
	float m00() const {return m_matrixElement[0];}
	float m01() const {return m_matrixElement[1];}
	float m02() const {return m_matrixElement[2];}
	float m03() const {return m_matrixElement[3];}
	float m10() const {return m_matrixElement[4];}
	float m11() const {return m_matrixElement[5];}
	float m12() const {return m_matrixElement[6];}
	float m13() const {return m_matrixElement[7];}
	float m20() const {return m_matrixElement[8];}
	float m21() const {return m_matrixElement[9];}
	float m22() const {return m_matrixElement[10];}
	float m23() const {return m_matrixElement[11];}
	float m30() const {return m_matrixElement[12];}
	float m31() const {return m_matrixElement[13];}
	float m32() const {return m_matrixElement[14];}
	float m33() const {return m_matrixElement[15];}


	// constructor - this is called when every new object is created
	Matrix4x4f()
	{
		resetToIdentity();
	}

	// setter method to change the cartesian X, Y and Z components all in one go
	//void set(float newX, float newY, float newZ)
	//{
	//	// for definition of methods x(), y(), and z() see 
	//	x() = newX;
	//	y() = newY;
	//	z() = newZ;
	//}

	inline void resetToIdentity(void)
	{
		m00() = 1; m01() = 0; m02() = 0; m03() = 0;
		m10() = 0; m11() = 1; m12() = 0; m13() = 0;
		m20() = 0; m21() = 0; m22() = 1; m23() = 0;
		m30() = 0; m31() = 0; m32() = 0; m33() = 1;		
	}


private:
	void setAsTranslationMatrix(const Vector3f &translation)
	{
		resetToIdentity();
		m03() = translation.x();
		m13() = translation.y();
		m23() = translation.z();
	}

	void setAsRotationMatrix(const float &angle, const Vector3f &axis)
	{
		// create a rotation matrix using angle-axis rotation
		// overwriting previous matrix values
		// angle is in radians

		Vector3f normalizedAxis;
		normalizedAxis = (1/(axis.length()) * axis);

		float s = sin(angle);
		float c = cos(angle);

		float ux = normalizedAxis.x();
		float uy = normalizedAxis.y();
		float uz = normalizedAxis.z();

		// column 0
		m00() = c + (1-c) * ux;
		m10() = (1-c) * ux*uy + s*uz;
		m20() = (1-c) * ux*uz - s*uy;
		m30() = 0;

		// column 1
		m01() = (1-c) * uy*ux - s*uz;
		m11() = c + (1-c) * uy*uy;
		m21() = (1-c) * uy*uz + s*ux;
		m31() = 0;

		// column 2
		m02() = (1-c) * uz*ux + s*uy;
		m12() = (1-c) * uz*uy - s*ux;
		m22() = c + (1-c) * uz*uz;
		m32() = 0;

		// column 3
		m03() = 0;
		m13() = 0;
		m23() = 0;
		m33() = 1;

	}

	void setAsScalingMatrix(const Vector3f &scaling)
	{
		resetToIdentity();
		m00() = scaling.x();
		m11() = scaling.y();
		m22() = scaling.z();
	}

public:

	void translate(const Vector3f &translation);

	void rotateDegrees(const float &angleDegrees, const Vector3f &axis);

	void rotateRadians(const float &angleRadians, const Vector3f &axis);

	void scale(const Vector3f &scaling);

	void transpose(void)
	{
		Matrix4x4f temp;
		// col 0
		temp.m00() = m00();
		temp.m10() = m01();
		temp.m20() = m02();
		temp.m30() = m03();
		
		// col 1
		temp.m01() = m10();
		temp.m11() = m11();
		temp.m21() = m12();
		temp.m31() = m13();
				
		// col 2
		temp.m02() = m20();
		temp.m12() = m21();
		temp.m22() = m22();
		temp.m32() = m23();
				
		// col 3
		temp.m03() = m30();
		temp.m13() = m31();
		temp.m23() = m32();
		temp.m33() = m33();
	
		(*this) = temp;
	}

	// overload the = operator so we can set one matrix to another one
	// declaring the input matrix as 'const' indicates we wont change it
	void operator=(const Matrix4x4f other)
	{
		m00() = other.m00(); m01() = other.m01(); m02() = other.m02(); m03() = other.m03();
		m10() = other.m10(); m11() = other.m11(); m12() = other.m12(); m13() = other.m13();
		m20() = other.m20(); m21() = other.m21(); m22() = other.m22(); m23() = other.m23();
		m30() = other.m30(); m31() = other.m31(); m32() = other.m32(); m33() = other.m33();			
	}

}; // this is the end of the class definition - its member variables and methods

// non-member methods (functions) can also be defined, these are more intuitive for some operators

// For example, adding two vectors to produce a third vector is handled better by a non-member function
// here using the overloaded + operator.  Declaring this function 'inline' suggests to the compiler 
// to paste this code into wherever the function is called, rather than jump here every time.
// The compiler does not have to obey the 'inline' keyword.

inline const Vector3f operator*(const Matrix4x4f& matrix, const Vector3f& vector)
{
	Vector3f newVec;
	return newVec;	
}

inline const Vector3f operator*(const Vector3f& vector, const Matrix4x4f& matrix)
{
	Vector3f newVec;
	return newVec;	
}

inline const Matrix4x4f operator*(const Matrix4x4f &l, const Matrix4x4f &r)
{
	Matrix4x4f newMat;
	// row 0
	newMat.m00() = l.m00()*r.m00() + l.m01()*r.m10() + l.m02()*r.m20() + l.m03()*r.m30();
	newMat.m01() = l.m00()*r.m01() + l.m01()*r.m11() + l.m02()*r.m21() + l.m03()*r.m31();
	newMat.m02() = l.m00()*r.m02() + l.m01()*r.m12() + l.m02()*r.m22() + l.m03()*r.m32();
	newMat.m03() = l.m00()*r.m03() + l.m01()*r.m13() + l.m02()*r.m23() + l.m03()*r.m33();

	// row 1
	newMat.m10() = l.m10()*r.m00() + l.m11()*r.m10() + l.m12()*r.m20() + l.m13()*r.m30();
	newMat.m11() = l.m10()*r.m01() + l.m11()*r.m11() + l.m12()*r.m21() + l.m13()*r.m31();
	newMat.m12() = l.m10()*r.m02() + l.m11()*r.m12() + l.m12()*r.m22() + l.m13()*r.m32();
	newMat.m13() = l.m10()*r.m03() + l.m11()*r.m13() + l.m12()*r.m23() + l.m13()*r.m33();

	// row 2
	newMat.m20() = l.m20()*r.m00() + l.m21()*r.m10() + l.m22()*r.m20() + l.m23()*r.m30();
	newMat.m21() = l.m20()*r.m01() + l.m21()*r.m11() + l.m22()*r.m21() + l.m23()*r.m31();
	newMat.m22() = l.m20()*r.m02() + l.m21()*r.m12() + l.m22()*r.m22() + l.m23()*r.m32();
	newMat.m23() = l.m20()*r.m03() + l.m21()*r.m13() + l.m22()*r.m23() + l.m23()*r.m33();

	// row 3
	newMat.m30() = l.m30()*r.m00() + l.m31()*r.m10() + l.m32()*r.m20() + l.m33()*r.m30();
	newMat.m31() = l.m30()*r.m01() + l.m31()*r.m11() + l.m32()*r.m21() + l.m33()*r.m31();
	newMat.m32() = l.m30()*r.m02() + l.m31()*r.m12() + l.m32()*r.m22() + l.m33()*r.m32();
	newMat.m33() = l.m30()*r.m03() + l.m31()*r.m13() + l.m32()*r.m23() + l.m33()*r.m33();

	return newMat;
}

