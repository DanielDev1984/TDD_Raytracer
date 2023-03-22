#pragma once
#include "ArithmeticStructures.h"
#include "Ray.h"
#include <vector>
class GeometricStructures
{
public:
	using Intersections = std::vector<float>;
	struct Sphere {
		
		Sphere(ArithmeticStructures::HomogenousCoordinates origin,int radius) : 
			m_origin{origin},
			m_radius{ radius } {};
		int getRadius() { return m_radius; };
		ArithmeticStructures::HomogenousCoordinates getOrigin() { return m_origin; };
		Intersections getIntersections(Ray ray) { return m_intersections; };
	private:
		int m_radius{};
		ArithmeticStructures::HomogenousCoordinates m_origin{};
		 Intersections m_intersections{};
	};
};

