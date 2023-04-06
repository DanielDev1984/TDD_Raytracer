#include "SceneObject.h"
#include <algorithm>
SceneObject::Intersections SceneObject::getSphereIntersections(Ray ray)
{
	// !! this intersection algorithm treats the ray as a line and will also return an intersection when the intersection lies "behind" the ray origin

	auto tmpTransformation{ getSphereTransformation() };
	ArithmeticStructures::inverseMatrix(tmpTransformation);
	auto scaledRayLocal{ ray.scale(tmpTransformation) }; //todo: right now only scaling is considered -> consider all types of transformations!

	// vector pointing from the center of the sphere to the origin of the ray
	const auto vec_sphereToRay{ ArithmeticStructures::subtractCoordinates(scaledRayLocal.getOrigin(), m_sphereGeo.getOrigin()) };
	const auto a{ ArithmeticStructures::dotProduct(scaledRayLocal.getDirection(), scaledRayLocal.getDirection()) };
	const auto b{ 2 * (ArithmeticStructures::dotProduct(scaledRayLocal.getDirection(), vec_sphereToRay)) };
	const auto c{ ArithmeticStructures::dotProduct(vec_sphereToRay, vec_sphereToRay) - 1.0 };

	// standard way of determining LINE (!= ray) sphere intersection
	const auto discriminant{ pow(b,2) - 4 * a * c };

	// no intersections between ray and spehere if discriminant < 0

	if (discriminant >= 0)
	{
		// always calculate 2 intersections even if the sphere is only hit tangentially or the origin of the ray lies within the sphere
		const auto t1{ (-1.0 * b - sqrt(discriminant)) / (2 * a) };
		const auto t2{ (-1.0 * b + sqrt(discriminant)) / (2 * a) };
		m_intersections.at(0) = t1;
		m_intersections.at(1) = t2;
	}
	else
	{
		// somehow it has to be indicated that there is no intersection 
		m_intersections.fill(Invalid); //todo: maybe fill with NAN instead?!
	}

	return m_intersections;

};

float SceneObject::getSphereHit(Ray ray)
{
	float hit{ NAN }; // init with NAN to indicate on the call-site whether there are hits at all
	std::vector<float> potentialHits{};
	potentialHits.clear();
	const auto sphereIntersections{ getSphereIntersections(ray) };
	// first identify potential hits 
	for (const auto & currentSphereIntersection : sphereIntersections)
	{
		if (currentSphereIntersection >= 0.0 && currentSphereIntersection != Invalid)
		{
			potentialHits.push_back(currentSphereIntersection);
		}
	}
	if (!potentialHits.empty())
	{
		if (potentialHits.size() > 1)
		{
			
			hit = std::min(potentialHits.at(0), potentialHits.at(1));
		}
		else
		{
			hit = potentialHits.at(0);
		}
	}
	return hit;
}
