#include "Canvas.h"

Canvas::Canvas(int dimX, int dimY) : m_x{ dimX }, m_y{ dimY } { m_imageData.resize(dimX); for (auto x{ 0 }; x < dimX; x++) { m_imageData[x].resize(dimY); } };

bool Canvas::setImageData(int x, int y, ArithmeticStructures::HomogenousCoordinates color) {
	bool coordinatesInCanvasaBounds{ (x >= 0 && x < m_x) && (y >= 0 && y < m_y) };
	if (coordinatesInCanvasaBounds)
	{
		const auto& [r, g, b, a] = color; m_imageData.at(x).at(y) = std::make_tuple(r, g, b);
	}
	return coordinatesInCanvasaBounds;
}

ArithmeticStructures::HomogenousCoordinates Canvas::getImageData(int x, int y) 
{ 
	const auto& [r, g, b] = m_imageData.at(x).at(y); return ArithmeticStructures::HomogenousCoordinates{ r,g,b,1.0 }; 
}