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
		// calculate the distance that is travelled in a certain ammount of time
		const auto& [x, y, z, w] = ArithmeticStructures::multiplyWithScalar(getDirection(), t);
		// "convert" this distance in a translation(matrix)
		const auto& translationMatrix{ ArithmeticStructures::getTranslationMatrix(x,y,z) };
		// and apply this translation to the rays origin / calculate the shifted point
		return { ArithmeticStructures::multiplyMatrixWithTuple(translationMatrix, getOrigin()) };
	};
private:
	ArithmeticStructures m_as; //todo: not sure whether this really is a good implementation...
};

