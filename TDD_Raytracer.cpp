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
	raytracer.calculateProjectilePathway();

	

}   

void TDD_Raytracer::calculateProjectilePathway()
{
	Canvas referenceCanvas(256, 256);
	PPMWriter imageWriter{ referenceCanvas.getDimX(), referenceCanvas.getDimY(), "test.ppm" };

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
