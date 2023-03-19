// TDD_Raytracer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "TDD_Raytracer.h"
#include "ArithmeticStructures.h"
#include "PPMWriter.h"
#include "Canvas.h"

int main()
{
	TDD_Raytracer raytracer{};
	//raytracer.calculateAndDrawProjectilePathway();
	raytracer.calculateAndDrawClock();
	

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