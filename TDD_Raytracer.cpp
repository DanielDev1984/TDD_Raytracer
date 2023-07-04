// TDD_Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include <iostream>
#include <string>
#include "TDD_Raytracer.h"
#include "ArithmeticStructures.h"
#include "PPMWriter.h"
#include "Canvas.h"
#include "GeometricStructures.h"
#include "SceneObject.h"
#include "QtWidget_VtkSandbox.h"
#include <QtWidgets/QApplication>

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	QtWidget_VtkSandbox w;
	w.show();
	return a.exec();
}   

void TDD_Raytracer::calculateAndDrawProjectilePathway()
{
	Canvas referenceCanvas(256, 256);
	PPMWriter imageWriter{ referenceCanvas.getDimX(), referenceCanvas.getDimY(), "projectile.ppm" };

	ArithmeticStructures projectile{};
	// set startpoint
	projectile.setPoint(0.0, 0.0, 0.0);
	// set velocity
	projectile.setVector(6.0, 20.0, 0.0);

	ArithmeticStructures gravity{};
	gravity.setVector(0.0, -0.981, 0.0);


	for (auto x = 0; x < referenceCanvas.getDimX(); x++)
	{
		for (auto y = 0; y < referenceCanvas.getDimY(); y++)
		{
			// color gradient
			//referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ (int)(x * 255.0 / referenceCanvas.getDimX()),(int)(0.0),(int)0.0,1.0 });
			// currentPosition
			const auto& [xCurrent, yCurrent, zCurrent, posNotUsed] = projectile.getPoint();
			referenceCanvas.setImageData((int)xCurrent, (int)yCurrent, ArithmeticStructures::HomogenousCoordinates{ (int)(255.0),(int)(0.0),(int)0.0,1.0 });
			// next iteration

			const auto& [calcX, calcY, calcZ, nA2] = ArithmeticStructures::addCoordinates(gravity.getVector(), projectile.getVector());
			// update the projectiles speed
			projectile.setVector(calcX, calcY, calcZ);
			// retrieve the updated speed of the projectile
			const auto& [newVX, newVY, newVZ, nA] = projectile.getVector();
			// calculate new position based on updated projectile properties
			projectile.setPoint(xCurrent + newVX, yCurrent + newVY, 0.0);

		}
	}

	imageWriter.createPPM(referenceCanvas);
}

void TDD_Raytracer::calculateAndDrawClock()
{
	Canvas referenceCanvas(256, 256);
	PPMWriter imageWriter{ referenceCanvas.getDimX(), referenceCanvas.getDimY(), "clock.ppm" };

	// setting up the clocks attributes (invariants)
	ArithmeticStructures centerPoint{}; //todo: how to make this const qualified?
	centerPoint.setPoint(referenceCanvas.getDimX() / 2.0, referenceCanvas.getDimY() / 2.0, 0.0);
	const float clockDiameter{ float(referenceCanvas.getDimX() / 2.0) };
	const float clockRadius{ float(clockDiameter / 2.0) };
	constexpr int pointHalo{ 1 }; // how many pixels around the acutally calculated point on the radius of the clock shall be colored
	//todo: first only draw a dot for every three hours
	constexpr int numberOfClockSegments{ 12 };
	constexpr float circleStepInRads{ 2.0 * M_PI / numberOfClockSegments };
	const ArithmeticStructures::Matrix4x4 rotationMatrix{ ArithmeticStructures::getRotationMatrix_ZAxis(circleStepInRads) };
	


	auto const [centerPoint_x, centerPoint_y, centerPoint_z, centerPoint_w] = centerPoint.getPoint();
	const ArithmeticStructures::Matrix4x4 translationMatrix{ ArithmeticStructures::getTranslationMatrix(centerPoint_x, centerPoint_y, 0.0) };

	// clock startPoint at 2PI / 12h
	const ArithmeticStructures::HomogenousCoordinates startPoint12h{ centerPoint_x, centerPoint_y - clockRadius, 0.0,1.0 };
	// initialize with startPoint
	ArithmeticStructures::HomogenousCoordinates currentPoint{ 0, clockRadius, 0.0,1.0 }; 
	auto& [x, y, z, w] = currentPoint;
		// calculate the position of the relevant points in the final image
	std::array<ArithmeticStructures::HomogenousCoordinates, numberOfClockSegments> pointsOnClock{};
	// first calculate the points wrt origin of coordinate system
	for (auto& currentSegment : pointsOnClock)
	{
		
		currentSegment = ArithmeticStructures::HomogenousCoordinates{ x, y, 0, 1};
		// iterate
		const auto nextPoint{ ArithmeticStructures::multiplyMatrixWithTuple(rotationMatrix, currentSegment) };
		const auto [nextPoint_x, nextPoint_y, nextPoint_z, nextPoint_w] = nextPoint;
		x = nextPoint_x;
		y = nextPoint_y;
	}

	// ... and then move the points to the center of the clock on the final image
	for (auto& currentSegment : pointsOnClock)
	{
		const auto translatedPoint{ ArithmeticStructures::multiplyMatrixWithTuple(translationMatrix, currentSegment) };
		const auto [translatedPoint_x, translatedPoint_y, translatedPoint_z, translatedPoint_w] = translatedPoint;
		currentSegment = ArithmeticStructures::HomogenousCoordinates{ translatedPoint_x, translatedPoint_y, 0, 1 };
		
		
	}


	bool currentImagePointIsPartOfClock{ false };
	for (auto x = 0; x < referenceCanvas.getDimX(); x++)
	{
		for (auto y = 0; y < referenceCanvas.getDimY(); y++)
		{
			// check if current point is part of the list of clocksegments
			for (const auto& currentPoint : pointsOnClock)
			{
				currentImagePointIsPartOfClock = false;
				const auto& [currentPt_X, currentPt_Y, currentPt_Z, currentPt_W] = currentPoint;
				if (((int)currentPt_X == x) && ((int)currentPt_Y == y) ||
					((((int)currentPt_X-pointHalo) == x) && (((int)currentPt_Y-pointHalo) == y)) ||
					((((int)currentPt_X + pointHalo) == x) && (((int)currentPt_Y + pointHalo) == y)) ||
					((((int)currentPt_X - pointHalo) == x) && (((int)currentPt_Y + pointHalo) == y)) || 
					((((int)currentPt_X + pointHalo) == x) && (((int)currentPt_Y - pointHalo) == y)) ||
					((((int)currentPt_X + pointHalo) == x) && (((int)currentPt_Y) == y)) ||
					((((int)currentPt_X - pointHalo) == x) && (((int)currentPt_Y) == y)) ||
					((((int)currentPt_X ) == x) && (((int)currentPt_Y + pointHalo) == y)) ||
					((((int)currentPt_X) == x) && (((int)currentPt_Y - pointHalo) == y)))
				{
					currentImagePointIsPartOfClock = true;
					break;
				}
			}
			if (currentImagePointIsPartOfClock)
			{
				referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ (int)(255.0),(int)(0.0),(int)(255.0-x),1.0 });
			}
			else
			{
				referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ x,(int)(0.0),x,1.0 });
			}
		}
	}

	imageWriter.createPPM(referenceCanvas);
}

void TDD_Raytracer::drawSphereWithBasicShading()
{
	Canvas referenceCanvas(256, 256);
	PPMWriter imageWriter{ referenceCanvas.getDimX(), referenceCanvas.getDimY(), "sphereWithBasicShading.ppm" };
	
	const ArithmeticStructures::HomogenousCoordinates sphere_Origin{ 0.0,0.0,0.0,1.0 };
	constexpr int sphere_Radius{ 1 };
	GeometricStructures::Sphere sphere{ sphere_Origin, sphere_Radius };
	SceneObject sO{ sphere };
	const float uniformSphereScale{ referenceCanvas.getDimX() / 2.0f };
	const float scale_x{ uniformSphereScale }, scale_y{ uniformSphereScale }, scale_z{ uniformSphereScale};
	const float shift_x{ 1.0f }, shift_y{  1.0f }, shift_z{ 1.0 }; //todo: why is 1.0 enough to move the sphere to the center???
	sO.setSphereScaling(ArithmeticStructures::getScalingMatrix(scale_x, scale_y, scale_z));
	sO.setSphereTranslation(ArithmeticStructures::getTranslationMatrix(shift_x, shift_y, shift_z));

	std::vector<float> hitValues{};
	const auto imageSize{ referenceCanvas.getDimX() * referenceCanvas.getDimY() };
	int progress{ 0 };
	std::cout << "starting raytracing (sphere with basic shading)\n";

	for (auto x = 0; x < referenceCanvas.getDimX(); x++)
	{
		for (auto y = 0; y < referenceCanvas.getDimY(); y++)
		{
			// ray is in front of the sphere!
			const ArithmeticStructures::HomogenousCoordinates origin{ x,y,0.0,1.0 };
			const ArithmeticStructures::HomogenousCoordinates direction{ 0.0,0.0,1.0,0.0 };
			Ray ray{ origin, direction };

			auto actualHit{ sO.getSphereHit(ray) };
			
			if(!std::isnan(actualHit))
			{
				referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ 255-(int)(actualHit),(int)(0.0),(int)(0.0),1.0 });
			}
			else
			{
				referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ (int)(0.0),(int)(0.0),(int)(0.0),1.0 });
			}
			// convenience function for debug / progress output
			progress = int((x * referenceCanvas.getDimY() + y ) * 100 / imageSize);
			updateProgressBar(progress);
		}
	}
	std::cout << "\nend raytracing, write ppm";
	imageWriter.createPPM(referenceCanvas);
}

void TDD_Raytracer::drawSphereWithPhongShading(const float lightPos_x, const float lightPos_y, const float lightPos_z, std::string fileName,bool useDimAsFileName, unsigned int color_r, unsigned int color_g, unsigned int color_b)
{
	Canvas referenceCanvas(256, 256);
	//const float light_x{512.0 }, light_y{ 256.0 }, light_z{ -64.0 };
	const float light_x{ lightPos_x }, light_y{ lightPos_y }, light_z{ lightPos_z };
	std::string fN{};
	if (useDimAsFileName)
	{
		fN += "X_" + std::to_string((int)light_x) + "Y_" + std::to_string((int)light_y) + "Z_" + std::to_string((int)light_z) + ".ppm";
	}
	else
	{
		fN = fileName + ".ppm";
	}
	//PPMWriter imageWriter{ referenceCanvas.getDimX(), referenceCanvas.getDimY(), "sphereWithPhongShading.ppm" };
	PPMWriter imageWriter{ referenceCanvas.getDimX(), referenceCanvas.getDimY(), fN };

	const ArithmeticStructures::HomogenousCoordinates sphere_Origin{ 0.0,0.0,0.0,1.0 };
	constexpr int sphere_Radius{ 1 };
	GeometricStructures::Sphere sphere{ sphere_Origin, sphere_Radius };
	SceneObject sO{ sphere };
	const float uniformSphereScale{ float(referenceCanvas.getDimX()*0.5) };
	const float scale_x{ uniformSphereScale }, scale_y{ uniformSphereScale }, scale_z{ uniformSphereScale };
	const float shift_x{ 1.0f }, shift_y{0.0f }, shift_z{1.0f }; //todo: why is 1.0 enough to move the sphere to the center???
	sO.setSphereScaling(ArithmeticStructures::getScalingMatrix(scale_x, scale_y, scale_z));
	sO.setSphereTranslation(ArithmeticStructures::getTranslationMatrix(shift_x, shift_y, shift_z));

	constexpr float ambientFactor{ 0.3 }, diffuseFactor{ 0.9 }, specularFactor{ 0.0 }, shininessFactor{ 0.0 };
	
	const auto valR_normalized{ float(color_r / 255.0) };
	const auto valG_normalized{ float(color_g / 255.0) };
	const auto valB_normalized{ float(color_b / 255.0) };
	
	const ArithmeticStructures::HomogenousCoordinates color{valR_normalized , valG_normalized,valB_normalized,1.0 };
	Material m{ ambientFactor, diffuseFactor, specularFactor, shininessFactor,color };
	sO.setSphereMaterial(m);

	const ArithmeticStructures::HomogenousCoordinates lightSourceIntensity{ 1.0,1.0,1.0,1.0 };
	const ArithmeticStructures::HomogenousCoordinates lightSourcePosition{ light_x,light_y,light_z,1.0 };
	SceneObject::LightSource lS{ lightSourceIntensity, lightSourcePosition };


	std::vector<float> hitValues{};
	const auto imageSize{ referenceCanvas.getDimX() * referenceCanvas.getDimY() };
	int progress{ 0 };
	std::cout << "starting raytracing (sphere with Phong shading)\n";
	std::cout << "output filename: " << fN << "\n";

	for (auto x = 0; x < referenceCanvas.getDimX(); x++)
	{
		for (auto y = 0; y < referenceCanvas.getDimY(); y++)
		{
			// ray is in front of the sphere!
			const ArithmeticStructures::HomogenousCoordinates origin{ x,y,0.0,1.0 };
			const ArithmeticStructures::HomogenousCoordinates direction{ 0.0,0.0,1.0,0.0 };
			Ray ray{ origin, direction };

			auto actualHit{ sO.getSphereHit(ray) };
			// we have a hit
			if (!std::isnan(actualHit))
			{
				auto hitPointCoordinates{ ArithmeticStructures::HomogenousCoordinates{x,y,actualHit,1.0} };
				
				

				auto surfaceNormalAtHitPoint{sO.getNormalOnSphereSurfaceAt(hitPointCoordinates)};
				auto eyeVector{ ArithmeticStructures::subtractCoordinates(hitPointCoordinates, ray.getOrigin()) };
				auto [a, b, c, d] = eyeVector;
				ArithmeticStructures aS_local{};
				aS_local.setVector(a, b, c);
				eyeVector = aS_local.getNormalizedVector();
				auto colorAtHitPoint{ sO.getPhongShadedSurfaceColor(m, lS, hitPointCoordinates, surfaceNormalAtHitPoint,eyeVector ) };
				referenceCanvas.setImageData(x, y, ArithmeticStructures::multiplyWithScalar(colorAtHitPoint, 255.0));
				//referenceCanvas.setImageData(x, y, ArithmeticStructures::multiplyWithScalar(ArithmeticStructures::HomogenousCoordinates{ (int)(0.0),255-(int)(actualHit),(int)(0.0),1.0 }, 1.0));
			}//todo: enable this branch again to calculate light direction / vector field lateron
			else if(false)// draw background
			{
				// todo: instead of trying to colorize the background, calculate a 2d vectorfield for every "pixel" on the imageplane, representing the direction of the lightsource. out of this an overlay could be calculated thar represents the lightsource as e.g. "arrows" or similar
				// 
				// 
				// 
				// the lightsource is visualized as a unidirectional (point)lightsource with decreasing luminance wrt 
				//const auto luminance{ (int)(255.0 * (x * 100.0 / 255.0) * 0.01) };
				ArithmeticStructures::HomogenousCoordinates lightOrigin{ light_x,light_y, 0.0, 1.0 };
				ArithmeticStructures::HomogenousCoordinates pointOnImageplane{ x,y, 0.0, 1.0 };
				auto vectorLightSourceToImagePoint{ ArithmeticStructures::subtractCoordinates(pointOnImageplane, lightOrigin) };
				ArithmeticStructures aS{};
				const auto& [x_local, y_local, z_local, w] = vectorLightSourceToImagePoint;
				aS.setVector(x_local,y_local,z_local);

				//auto luminance{ 600.0 - sqrt(pow(x_local, 2.0) + (y_local, 2.0)) };

				//todo: find a proper, dynamic value, that allows for a smooth, radial, gradient of the lightsource
				auto luminance{700.0 - (aS.magnitude())};
				luminance < 0 || luminance > 255 ? 
					referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ 255,0,0,1.0 }) : 
					referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ (int)(luminance),(int)(luminance),(int)(luminance),1.0 });
				//referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ (int)(luminance),(int)(luminance),(int)(luminance),1.0 });
				//const auto luminance{ 143.0 };
				//referenceCanvas.setImageData(x, y, ArithmeticStructures::HomogenousCoordinates{ (int)(luminance),(int)(luminance),(int)(luminance),1.0 });
			}
			// convenience function for debug / progress output
			progress = int((x * referenceCanvas.getDimY() + y) * 100 / imageSize);
			updateProgressBar(progress);
		}
	}
	std::cout << "\nend raytracing, write ppm" << "\n";
	imageWriter.createPPM(referenceCanvas);
}
//todo: update progressbar on qt-GUI (instead of console!)
void TDD_Raytracer::updateProgressBar(int progressPercentage)
{
	if (!m_progressBarInitialized)
	{
		initProgressBar();
		m_progressBarInitialized = true;
	}

	if ((progressPercentage % s_progressBarStepSize == 0) && (m_progress != progressPercentage))
	{
		m_progress = progressPercentage;
		std::cout << "|" << " " << progressPercentage;
	}
	
}