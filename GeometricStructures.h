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
		Intersections getIntersections(Ray ray) { 
			// !! this intersection algorithm treats the ray as a line and will also return an intersection when the intersection lies "behind" the ray origin

			// vector pointing from the center of the sphere to the origin of the ray
			const auto vec_sphereToRay{ArithmeticStructures::subtractCoordinates(ray.getOrigin(), this->getOrigin())};
			const auto a{ ArithmeticStructures::dotProduct(ray.getDirection(), ray.getDirection()) };
			const auto b{ 2 * (ArithmeticStructures::dotProduct(ray.getDirection(), vec_sphereToRay)) };
			const auto c{ ArithmeticStructures::dotProduct(vec_sphereToRay, vec_sphereToRay) -1.0 };
			
			// standard way of determining LINE (!= ray) sphere intersection
			const auto discriminant{pow(b,2) - 4*a*c};

			// no intersections between ray and spehere if discriminant < 0
			m_intersections.clear();
			if (discriminant >= 0)
			{
				// always calculate 2 intersections even if the sphere is only hit tangentially or the origin of the ray lies within the sphere
				const auto t1{(-1.0*b - sqrt(discriminant)) / (2*a) };
				const auto t2{ (-1.0 * b + sqrt(discriminant)) / (2 * a) };
				m_intersections.push_back(t1);
				m_intersections.push_back(t2);
			}

			return m_intersections; 
		
		};
	private:
		int m_radius{};
		ArithmeticStructures::HomogenousCoordinates m_origin{};
		 Intersections m_intersections{};
	};
};

