#pragma once
#include "ArithmeticStructures.h"
#include "GeometricStructures.h"
#include "Ray.h"
#include <array>
class SceneObject
{
public:
	using Intersections = std::array<float,2>;
	SceneObject(GeometricStructures::Sphere geo) :m_sphereGeo{ GeometricStructures::Sphere(geo.getOrigin(), geo.getRadius()) }, m_scaleTransformation{ ArithmeticStructures::getIdentityMatrix() }, m_shifTransformation{ ArithmeticStructures::getIdentityMatrix() } {};
	Intersections getSphereIntersections(Ray ray);
	ArithmeticStructures::Matrix4x4 getSphereScaling() { return m_scaleTransformation; };
	void setSphereScaling(ArithmeticStructures::Matrix4x4 scaleTransformation) { m_scaleTransformation = scaleTransformation; };
	ArithmeticStructures::Matrix4x4 getSphereTranslation() { return m_shifTransformation; };
	void setSphereTranslation(ArithmeticStructures::Matrix4x4 shiftTransformation) { m_shifTransformation = shiftTransformation; };
	float getSphereHit(Ray ray);
	ArithmeticStructures::HomogenousCoordinates getNormalOnSphereSurfaceAt(ArithmeticStructures::HomogenousCoordinates pointOnSphere) { return ArithmeticStructures::HomogenousCoordinates(0.0, 0.0, 0.0, 0.0); };

	static constexpr float Invalid{ -123456.0 };
private:
	Intersections m_intersections{};
	GeometricStructures::Sphere m_sphereGeo;
	ArithmeticStructures::Matrix4x4 m_scaleTransformation;
	ArithmeticStructures::Matrix4x4 m_shifTransformation;
};

