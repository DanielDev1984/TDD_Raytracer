#pragma once

#include "ArithmeticStructures.h"
class Ray
{
public:
	Ray(ArithmeticStructures::HomogenousCoordinates rayOrigin, ArithmeticStructures::HomogenousCoordinates rayDirection);


	ArithmeticStructures::HomogenousCoordinates getOrigin() { return m_as.getPoint(); };
	void setOrigin(float x, float y, float z) { m_as.setPoint(x, y, z); }
	ArithmeticStructures::HomogenousCoordinates getDirection() { return m_as.getVector(); };
	ArithmeticStructures::HomogenousCoordinates getPosition(float t);
	Ray translate(float shift_x, float shift_y, float shift_z);
	Ray scale(float scale_x, float scale_y, float scale_z);
	Ray scale(ArithmeticStructures::Matrix4x4);
private:
	ArithmeticStructures m_as; //todo: not sure whether this really is a good implementation...
};

