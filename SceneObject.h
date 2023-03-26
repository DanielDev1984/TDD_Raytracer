#pragma once
#include "ArithmeticStructures.h"
#include "GeometricStructures.h"
#include "Ray.h"
#include <array>
class SceneObject
{
public:
	using Intersections = std::array<float,2>;
	SceneObject(GeometricStructures::Sphere geo) :m_sphereGeo{ GeometricStructures::Sphere(geo.getOrigin(), geo.getRadius()) } {};
	Intersections getSphereIntersections(Ray ray);
	float getSphereHit(Ray ray);
	static constexpr float Invalid{ -123456.0 };
private:
	Intersections m_intersections{};
	GeometricStructures::Sphere m_sphereGeo;
};

