#pragma once
#include "ArithmeticStructures.h"
#include "GeometricStructures.h"
#include "Ray.h"
class SceneObject
{
public:
	using Intersections = std::vector<float>;
	SceneObject(GeometricStructures::Sphere geo) :m_sphereGeo{ GeometricStructures::Sphere(geo.getOrigin(), geo.getRadius()) } {};
	Intersections getSphereIntersections(Ray ray);
private:
	Intersections m_intersections{};
	GeometricStructures::Sphere m_sphereGeo;
};

