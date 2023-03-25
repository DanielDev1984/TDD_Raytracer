#pragma once
#include <vector>
#include "ArithmeticStructures.h"
class Canvas
{
public:
	Canvas(int dimX, int dimY);
	int getDimX() { return m_x; };
	int getDimY() { return m_y; };
	bool setImageData(int x, int y, ArithmeticStructures::HomogenousCoordinates color);

	ArithmeticStructures::HomogenousCoordinates getImageData(int x, int y);
	

private:
	int m_x{ 0 };
	int m_y{ 0 };
	std::vector<std::vector<std::tuple<float,float,float>>> m_imageData;
};

