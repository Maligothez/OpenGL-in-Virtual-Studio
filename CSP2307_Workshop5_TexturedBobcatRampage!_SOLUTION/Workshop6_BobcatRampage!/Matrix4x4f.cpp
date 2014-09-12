#include "Matrix4x4f.h"


void Matrix4x4f::translate(const Vector3f &translation)
	{
		Matrix4x4f newMatrix;
		newMatrix.setAsTranslationMatrix(translation);
		

		Matrix4x4f testMatrix;

		(*this) = newMatrix * (*this);
	}


	void Matrix4x4f::rotateDegrees(const float &angleDegrees, const Vector3f &axis)
	{
		float angleRadians = angleDegrees * DEG_TO_RAD;
		Matrix4x4f newMatrix;
		newMatrix.setAsRotationMatrix(angleRadians, axis);
		(*this) = newMatrix * (*this);
	}

	void Matrix4x4f::rotateRadians(const float &angleRadians, const Vector3f &axis)
	{
		Matrix4x4f newMatrix;
		newMatrix.setAsRotationMatrix(angleRadians, axis);
		(*this) = newMatrix * (*this);
	}

	void Matrix4x4f::scale(const Vector3f &scaling)
	{
		Matrix4x4f newMatrix;
		newMatrix.setAsScalingMatrix(scaling);
		(*this) = newMatrix * (*this);
	}

