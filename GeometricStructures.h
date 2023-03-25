#pragma once
#include "ArithmeticStructures.h"
class GeometricStructures
{
public:
	struct Sphere {
		
		Sphere(ArithmeticStructures::HomogenousCoordinates origin,int radius) : 
			m_origin{origin},
			m_radius{ radius } {};
		int getRadius() { return m_radius; };
		ArithmeticStructures::HomogenousCoordinates getOrigin() { return m_origin; };

	private:
		int m_radius{};
		ArithmeticStructures::HomogenousCoordinates m_origin{};
	};
};

