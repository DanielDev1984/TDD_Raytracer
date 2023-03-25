#pragma once
#include "ArithmeticStructures.h"
#include "GeometricStructures.h"
#include "Ray.h"
class SceneObject
{
public:
	using Intersections = std::vector<float>;
	SceneObject(GeometricStructures::Sphere geo) :m_geo{ GeometricStructures::Sphere(geo.getOrigin(), geo.getRadius()) } {};
	Intersections getIntersections(Ray ray) {
		// !! this intersection algorithm treats the ray as a line and will also return an intersection when the intersection lies "behind" the ray origin

		// vector pointing from the center of the sphere to the origin of the ray
		const auto vec_sphereToRay{ ArithmeticStructures::subtractCoordinates(ray.getOrigin(), m_geo.getOrigin()) };
		const auto a{ ArithmeticStructures::dotProduct(ray.getDirection(), ray.getDirection()) };
		const auto b{ 2 * (ArithmeticStructures::dotProduct(ray.getDirection(), vec_sphereToRay)) };
		const auto c{ ArithmeticStructures::dotProduct(vec_sphereToRay, vec_sphereToRay) - 1.0 };

		// standard way of determining LINE (!= ray) sphere intersection
		const auto discriminant{ pow(b,2) - 4 * a * c };

		// no intersections between ray and spehere if discriminant < 0
		m_intersections.clear();
		if (discriminant >= 0)
		{
			// always calculate 2 intersections even if the sphere is only hit tangentially or the origin of the ray lies within the sphere
			const auto t1{ (-1.0 * b - sqrt(discriminant)) / (2 * a) };
			const auto t2{ (-1.0 * b + sqrt(discriminant)) / (2 * a) };
			m_intersections.push_back(t1);
			m_intersections.push_back(t2);
		}

		return m_intersections;

	};
private:
	Intersections m_intersections{};
	GeometricStructures::Sphere m_geo;
};

