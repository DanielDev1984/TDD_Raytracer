#include "Ray.h"

Ray::Ray(ArithmeticStructures::HomogenousCoordinates rayOrigin, ArithmeticStructures::HomogenousCoordinates rayDirection) {
	const auto& [orig_x, orig_y, orig_z, orig_w] = rayOrigin;
	const auto& [dir_x, dir_y, dir_z, dir_w] = rayDirection;
	setOrigin(orig_x, orig_y, orig_z);
	m_as.setVector(dir_x, dir_y, dir_z);
};

ArithmeticStructures::HomogenousCoordinates Ray::getPosition(float t) {
	// calculate the distance that is travelled in a certain ammount of time
	const auto& [x, y, z, w] = ArithmeticStructures::multiplyWithScalar(getDirection(), t);
	// "convert" this distance in a translation(matrix)
	const auto& translationMatrix{ ArithmeticStructures::getTranslationMatrix(x,y,z) };
	// and apply this translation to the rays origin / calculate the shifted point
	return { ArithmeticStructures::multiplyMatrixWithTuple(translationMatrix, getOrigin()) };
};

Ray Ray::translate(float shift_x, float shift_y, float shift_z)
{
	auto transformationMatrix{ ArithmeticStructures::getTranslationMatrix(shift_x,shift_y,shift_z) };
	const auto shiftedOrigin{ ArithmeticStructures::multiplyMatrixWithTuple(transformationMatrix, getOrigin()) };

	return Ray{ shiftedOrigin, getDirection() };
}

Ray Ray::scale(float scale_x, float scale_y, float scale_z)
{
	auto transformationMatrix{ ArithmeticStructures::getScalingMatrix(scale_x,scale_y,scale_z) };
	const auto scaled_Origin{ ArithmeticStructures::multiplyMatrixWithTuple(transformationMatrix, getOrigin()) };
	const auto scaled_Direction{ ArithmeticStructures::multiplyMatrixWithTuple(transformationMatrix, getDirection()) };
	return Ray{scaled_Origin, scaled_Direction};
}