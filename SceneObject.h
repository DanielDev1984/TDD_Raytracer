#pragma once
#include "ArithmeticStructures.h"
#include "GeometricStructures.h"
#include "Ray.h"
#include <array>
class SceneObject
{
public:
	using Intersections = std::array<float,2>;
	SceneObject(GeometricStructures::Sphere geo) :m_sphereGeo{ GeometricStructures::Sphere(geo.getOrigin(), geo.getRadius()) }, m_scaleTransformation{ ArithmeticStructures::getIdentityMatrix() }, 
		m_shifTransformation{ ArithmeticStructures::getIdentityMatrix() },
		m_transformation{ ArithmeticStructures::getIdentityMatrix() }
	{};
	Intersections getSphereIntersections(Ray ray);
	ArithmeticStructures::Matrix4x4 getSphereScaling() { return m_scaleTransformation; };
	void setSphereScaling(ArithmeticStructures::Matrix4x4 scaleTransformation) { m_scaleTransformation = scaleTransformation; };
	ArithmeticStructures::Matrix4x4 getSphereTranslation() { return m_shifTransformation; };
	void setSphereTranslation(ArithmeticStructures::Matrix4x4 shiftTransformation) { m_shifTransformation = shiftTransformation; };

	void setSphereTransformation(ArithmeticStructures::Matrix4x4 transformation) { m_transformation = transformation; };

	float getSphereHit(Ray ray);
	// this function returns a normal vector in world coordinates at the point of interest
	ArithmeticStructures::HomogenousCoordinates getNormalOnSphereSurfaceAt(ArithmeticStructures::HomogenousCoordinates pointOnSphereInWorldCoordinates);
	ArithmeticStructures::HomogenousCoordinates getReflectedVectorAroundNormal(ArithmeticStructures::HomogenousCoordinates inVec, ArithmeticStructures::HomogenousCoordinates n);

	static constexpr float Invalid{ -123456.0 };
	
	//todo: outsource lightsource ?
	struct LightSource {
		LightSource(ArithmeticStructures::HomogenousCoordinates intensity, ArithmeticStructures::HomogenousCoordinates position) : m_intensity{ intensity }, m_position{ position } {};
		ArithmeticStructures::HomogenousCoordinates getIntensity() { return m_intensity; };
		ArithmeticStructures::HomogenousCoordinates getPosition() { return m_position; };
	private:
		ArithmeticStructures::HomogenousCoordinates m_intensity;
		ArithmeticStructures::HomogenousCoordinates m_position;
	};

private:
	Intersections m_intersections{};
	GeometricStructures::Sphere m_sphereGeo;
	//todo: remove dedicated members for different types of transformations
	ArithmeticStructures::Matrix4x4 m_scaleTransformation;
	ArithmeticStructures::Matrix4x4 m_shifTransformation;
	ArithmeticStructures::Matrix4x4 m_transformation;
};

