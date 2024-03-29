#include "SceneObject.h"
#include <algorithm>
SceneObject::Intersections SceneObject::getSphereIntersections(Ray ray)
{
	// !! this intersection algorithm treats the ray as a line and will also return an intersection when the intersection lies "behind" the ray origin

	auto scaleTransformation{ getSphereScaling() };
	ArithmeticStructures::inverseMatrix(scaleTransformation);
	auto scaledRayLocal{ ray.scale(scaleTransformation) };

	auto shiftTransformation{ getSphereTranslation() };
	ArithmeticStructures::inverseMatrix(shiftTransformation);
	auto scaledAndShiftedRayLocal{ scaledRayLocal.translate(shiftTransformation) }; 
	//todo: right now rotation is not considered!


	// vector pointing from the center of the sphere to the origin of the ray
	const auto vec_sphereToRay{ ArithmeticStructures::subtractCoordinates(scaledAndShiftedRayLocal.getOrigin(), m_sphereGeo.getOrigin()) };
	const auto a{ ArithmeticStructures::dotProduct(scaledAndShiftedRayLocal.getDirection(), scaledAndShiftedRayLocal.getDirection()) };
	const auto b{ 2 * (ArithmeticStructures::dotProduct(scaledAndShiftedRayLocal.getDirection(), vec_sphereToRay)) };
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

ArithmeticStructures::HomogenousCoordinates SceneObject::getPhongShadedSurfaceColor(
	Material m,
	SceneObject::LightSource l,
	ArithmeticStructures::HomogenousCoordinates pointOnSurface,
	ArithmeticStructures::HomogenousCoordinates surfaceNormalAtPointOfInterest,
	ArithmeticStructures::HomogenousCoordinates eyeVector)
{
	ArithmeticStructures::HomogenousCoordinates ambientColor{ 0.0,0.0,0.0,1.0 }, diffuseColor{ 0.0,0.0,0.0,1.0 }, specularColor{ 0.0,0.0,0.0,1.0 };
	auto combinedMaterialAndLightColor{ ArithmeticStructures::multiplyWithVector(m.getColor(), l.getColor()) };
	auto surfacePointToLightVector{ArithmeticStructures::subtractCoordinates(l.getPosition(),pointOnSurface)};
	ArithmeticStructures aS{};
	auto [x,y,z,w] = surfacePointToLightVector; //todo: make this procedure of creating a normal more convenient on ArithmeticStructures side !
	aS.setVector(x,y,z);
	auto normalizedSurfacePointToLightVector{ aS.getNormalizedVector() };
	ambientColor = ArithmeticStructures::multiplyWithScalar(combinedMaterialAndLightColor, m.getAmbientReflectionFactor()) ;
	auto relationLightToNormal{ArithmeticStructures::dotProduct(surfaceNormalAtPointOfInterest, normalizedSurfacePointToLightVector)};
	// if relationLightToNormal < 0, point is on other side of surface, no diffuse and no specular color
	if (relationLightToNormal >= 0)
	{
		// calculating final diffuseColor
		auto finalDiffuseFactor = m.getDiffuseReflectionFactor() * relationLightToNormal;
		diffuseColor = ArithmeticStructures::multiplyWithScalar(combinedMaterialAndLightColor, finalDiffuseFactor);

		// calculating final specularColor
		auto reflectedVector{getReflectedVectorAroundNormal(ArithmeticStructures::multiplyWithScalar(normalizedSurfacePointToLightVector,-1.0) ,surfaceNormalAtPointOfInterest)};
		auto relationEyeToReflectionVector{ArithmeticStructures::dotProduct(reflectedVector, eyeVector)};
		// if relationEyeToReflectionVector < 0, no specular color
		if (relationEyeToReflectionVector >= 0)
		{
			relationEyeToReflectionVector = pow(relationEyeToReflectionVector, m.getShininessFactor());
			auto finalSpecularFactor{ m.getSpecularReflectionFactor() * relationEyeToReflectionVector };
			specularColor = ArithmeticStructures::multiplyWithScalar(l.getColor(), finalSpecularFactor);
		}
	}
	/*else
	{
		diffuseColor = ArithmeticStructures::HomogenousCoordinates{ 1.0,0.0,0.0,1.0 };
	}*/
	// final Phong shaded color = ambient + diffuse + specular color
	
	auto finalColor{ ArithmeticStructures::addCoordinates(ArithmeticStructures::addCoordinates(ambientColor, diffuseColor), specularColor) };
	// reset w just to be on the safe side
	//todo: use a better function for adding colors / make sure the w component doesnt exceed 1.0
	auto& [finalColor_x, finalColor_y, finalColor_z, finalColor_w] = finalColor;
	finalColor_w = 1.0;
	return finalColor;
}

ArithmeticStructures::HomogenousCoordinates SceneObject::getNormalOnSphereSurfaceAt(ArithmeticStructures::HomogenousCoordinates pointOnSphereInWorldCoordinates)
{
	_ASSERT(m_sphereGeo.getRadius() == 1);

	// transform pointOnSphereInWorldCoordinates to object coordinate system
	auto invertedMatrix{ m_transformation };
	ArithmeticStructures::inverseMatrix(invertedMatrix);
	auto const pointOnSphereInObjectCoordinates{ ArithmeticStructures::multiplyMatrixWithTuple( invertedMatrix, pointOnSphereInWorldCoordinates) };
	// calculate normal in object coordinate system
	auto normalInObjectSpace{ ArithmeticStructures::subtractCoordinates(pointOnSphereInObjectCoordinates, m_sphereGeo.getOrigin()) };

	// transform normal from object coordinate system back to world coordinate system (taking stretichng etc into account!)
	ArithmeticStructures::transposeMatrix(invertedMatrix);
	auto normalInWorldSpace{ ArithmeticStructures::multiplyMatrixWithTuple(invertedMatrix, normalInObjectSpace) };

	//todo: calculating the normal needs to be optimized
	ArithmeticStructures aS;
	auto [x, y, z, w] = normalInWorldSpace;
	aS.setVector(x, y, z);

	// normalize vector AND hack for compensating messed w coordinate when translating
	auto [x_n, y_n, z_n, w_n] = aS.getNormalizedVector();
	
	normalInWorldSpace = ArithmeticStructures::HomogenousCoordinates{ x_n,y_n,z_n,0.0 };


	return normalInWorldSpace;
}

ArithmeticStructures::HomogenousCoordinates SceneObject::getReflectedVectorAroundNormal(ArithmeticStructures::HomogenousCoordinates inVec, ArithmeticStructures::HomogenousCoordinates n)
{
	// calculate the reflected vector according to this formula: in - (n * 2 * dot(in, n))
	auto dP{ ArithmeticStructures::dotProduct(inVec, n) };
	auto helperVector{ ArithmeticStructures::multiplyWithScalar(n, 2.0 * dP) };
	return ArithmeticStructures::subtractCoordinates(inVec, helperVector);
}
