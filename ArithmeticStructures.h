#pragma once
#include <tuple>
class ArithmeticStructures
{
	
public:

	
	using HomogenousCoordinates = std::tuple<float, float, float, const float>;
	
	// check for equality of coordinates, ignore the homogenous ("w") part
	static bool coordinatesAreEqual(const HomogenousCoordinates c1, const HomogenousCoordinates c2) {
		const auto& [x1, y1, z1, w1] = c1; const auto& [x2, y2, z2, w2] = c2; return (floatsAreEqual(x1,x2) && floatsAreEqual(y1,y2) && floatsAreEqual(z1,z2) && floatsAreEqual(w1,w2));
	};
	// add given coordinates and return resulting point or vector
	static HomogenousCoordinates addCoordinates(const HomogenousCoordinates& c1, const HomogenousCoordinates& c2) { const auto& [x1, y1, z1, w1] = c1; const auto& [x2, y2, z2, w2] = c2;
	return HomogenousCoordinates{ x1+x2,y1+y2,z1+z2, w1+w2 }; };
	//todo: add subtraction as well



	void setPoint(float x, float y, float z) { auto& [pX, pY, pZ, pW] = m_p; pX = x; pY = y; pZ = z; };
	HomogenousCoordinates getPoint() { return m_p; };
	void setVector(float x, float y, float z) { auto& [vX, vY, vZ, vW] = m_v; vX = x; vY = y; vZ = z; };
	HomogenousCoordinates getVector() { return m_v; };
	
	private:
		// helper function to properly compare floats
		static bool floatsAreEqual(float val1, float val2) { constexpr float epsilon{ 0.0001f }; return std::abs(val1 - val2) < epsilon; };
		// point
		HomogenousCoordinates m_p{ 0.0,0.0,0.0,1.0 };
		// vector
		HomogenousCoordinates m_v{ 0.0,0.0,0.0,.0 };
};

