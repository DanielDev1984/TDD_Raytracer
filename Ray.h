#pragma once

#include "ArithmeticStructures.h"
class Ray
{
public:
	Ray(ArithmeticStructures::HomogenousCoordinates rayOrigin, ArithmeticStructures::HomogenousCoordinates rayDirection) {
		const auto& [orig_x, orig_y, orig_z, orig_w] = rayOrigin;
		const auto& [dir_x, dir_y, dir_z, dir_w] = rayDirection;
		m_as.setPoint(orig_x, orig_y, orig_z);
		m_as.setVector(dir_x, dir_y, dir_z);
	};


	ArithmeticStructures::HomogenousCoordinates getOrigin() { return m_as.getPoint(); };
	ArithmeticStructures::HomogenousCoordinates getDirection() { return m_as.getVector(); };
	ArithmeticStructures::HomogenousCoordinates getPosition(float t) {
		return ArithmeticStructures::HomogenousCoordinates{ 0.0,0.0,0.0,1.0 };
	};
private:
	ArithmeticStructures m_as; //todo: not sure whether this really is a good implementation...
};

