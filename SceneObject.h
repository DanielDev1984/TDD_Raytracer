#pragma once
#include "ArithmeticStructures.h"
#include "GeometricStructures.h"
#include "Material.h"
#include "Ray.h"
#include <array>
class SceneObject
{
public:
	//todo: outsource lightsource ?
	struct LightSource {
		LightSource(ArithmeticStructures::HomogenousCoordinates color, ArithmeticStructures::HomogenousCoordinates position) : m_color{ color }, m_position{ position } {};
		ArithmeticStructures::HomogenousCoordinates getColor() { return m_color; }; //todo: add a variable that accounts explicitly for the intensity of the lightsource
		ArithmeticStructures::HomogenousCoordinates getPosition() { return m_position; };
		void setPosition(ArithmeticStructures::HomogenousCoordinates position) { m_position = position; };
	private:
		ArithmeticStructures::HomogenousCoordinates m_color;
		ArithmeticStructures::HomogenousCoordinates m_position;
	};

	using Intersections = std::array<float,2>;
	SceneObject(GeometricStructures::Sphere geo) :m_sphereGeo{ GeometricStructures::Sphere(geo.getOrigin(), geo.getRadius()) }, m_scaleTransformation{ ArithmeticStructures::getIdentityMatrix() }, 
		m_shifTransformation{ ArithmeticStructures::getIdentityMatrix() },
		m_transformation{ ArithmeticStructures::getIdentityMatrix()}, m_material{Material()} 
	{};
	Intersections getSphereIntersections(Ray ray);
	ArithmeticStructures::Matrix4x4 getSphereScaling() { return m_scaleTransformation; };
	void setSphereScaling(ArithmeticStructures::Matrix4x4 scaleTransformation) { m_scaleTransformation = scaleTransformation; };
	ArithmeticStructures::Matrix4x4 getSphereTranslation() { return m_shifTransformation; };
	void setSphereTranslation(ArithmeticStructures::Matrix4x4 shiftTransformation) { m_shifTransformation = shiftTransformation; };

	void setSphereTransformation(ArithmeticStructures::Matrix4x4 transformation) { m_transformation = transformation; };
	void setSphereMaterial(Material material) { m_material = material; };
	Material getSphereMaterial() { return m_material; };
	float getSphereHit(Ray ray);
	ArithmeticStructures::HomogenousCoordinates getPhongShadedSurfaceColor(
		Material m, 
		LightSource l, 
		ArithmeticStructures::HomogenousCoordinates pointOnSurface, 
		ArithmeticStructures::HomogenousCoordinates surfaceNormalAtPointOfInterest, 
		ArithmeticStructures::HomogenousCoordinates eyeVector);
	// this function returns a normal vector in world coordinates at the point of interest
	ArithmeticStructures::HomogenousCoordinates getNormalOnSphereSurfaceAt(ArithmeticStructures::HomogenousCoordinates pointOnSphereInWorldCoordinates);
	ArithmeticStructures::HomogenousCoordinates getReflectedVectorAroundNormal(ArithmeticStructures::HomogenousCoordinates inVec, ArithmeticStructures::HomogenousCoordinates n);

	static constexpr float Invalid{ -123456.0 };
	
	

private:
	Intersections m_intersections{};
	GeometricStructures::Sphere m_sphereGeo;
	//todo: remove dedicated members for different types of transformations
	ArithmeticStructures::Matrix4x4 m_scaleTransformation;
	ArithmeticStructures::Matrix4x4 m_shifTransformation;
	ArithmeticStructures::Matrix4x4 m_transformation;
	Material m_material{};
};

