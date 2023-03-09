#pragma once
#include <tuple>
#include <array>
#include <vector>
#include <algorithm>

class ArithmeticStructures
{
	
public:

	
	using HomogenousCoordinates = std::tuple<float, float, float, float>;
	
	// check for equality of coordinates, ignore the homogenous ("w") part
	static bool coordinatesAreEqual(const HomogenousCoordinates c1, const HomogenousCoordinates c2) {
		const auto& [x1, y1, z1, w1] = c1; const auto& [x2, y2, z2, w2] = c2; return (floatsAreEqual(x1,x2) && floatsAreEqual(y1,y2) && floatsAreEqual(z1,z2) && floatsAreEqual(w1,w2));
	};
	// add given coordinates and return resulting point or vector
	static HomogenousCoordinates addCoordinates(const HomogenousCoordinates& c1, const HomogenousCoordinates& c2) { const auto& [x1, y1, z1, w1] = c1; const auto& [x2, y2, z2, w2] = c2;
	return HomogenousCoordinates{ x1+x2,y1+y2,z1+z2, w1+w2 }; };
	//todo: add subtraction as well
	static HomogenousCoordinates multiplyWithScalar(const HomogenousCoordinates& c, const float s) {
		const auto& [x, y, z, w] = c;

		return HomogenousCoordinates{ x*s,y*s,z*s,w };
		
		 };
	static HomogenousCoordinates multiplyWithVector(const HomogenousCoordinates& c1, const HomogenousCoordinates& c2) {
		const auto& [x1, y1, z1, w1] = c1;
		const auto& [x2, y2, z2, w2] = c2;

		return HomogenousCoordinates{ x1*x2,y1*y2,z1*z2,0.0 };
	}
	static float dotProduct(const HomogenousCoordinates& c1, const HomogenousCoordinates& c2) { const auto& [x1, y1, z1, w1] = c1; const auto& [x2, y2, z2, w2] = c2; return (x1*x2+y1*y2+z1*z2+w1*w2); };
	static HomogenousCoordinates crossProduct(const HomogenousCoordinates& c1, const HomogenousCoordinates& c2) { 
		float xNew, yNew, zNew;
		constexpr float w{ 0.0 };
		const auto& [x1, y1, z1, w1] = c1;
		const auto& [x2, y2, z2, w2] = c2;
		xNew = y1 * z2 - z1 * y2;
		yNew = z1 * x2 - x1 * z2;
		zNew = x1 * y2 - y1 * x2;
		
		return HomogenousCoordinates{ xNew,yNew,zNew,w }; };
	float magnitude() { const auto& [x, y, z, w] = m_v; return sqrtf(x * x + y * y + z * z); };
	HomogenousCoordinates getNormalizedVector() { const float factor{ 1.0f / magnitude() }; const auto& [x, y, z,w] = getVector(); return HomogenousCoordinates{ x*factor,y*factor,z*factor,0.0 }; };

	void setPoint(float x, float y, float z) { auto& [pX, pY, pZ, pW] = m_p; pX = x; pY = y; pZ = z; };
	HomogenousCoordinates getPoint() { return m_p; };
	void setVector(float x, float y, float z) { auto& [vX, vY, vZ, vW] = m_v; vX = x; vY = y; vZ = z; };
	HomogenousCoordinates getVector() { return m_v; };
	
	
	
	

	enum class MatrixType {
		Matrix2x2 = 0,
		Matrix3x3,
		Matrix4x4,

	};
	using row2x2 = std::array<float, 2>;
	using row3x3 = std::array<float, 3>;
	using row4x4 = std::array<float, 4>;

	struct Matrix2x2 {
		Matrix2x2(row2x2 m0, row2x2 m1) : m_22{
			{ 
			{m0.at(0),m0.at(1)}, 
			{m1.at(0),m1.at(1)} }
	}, type{ MatrixType::Matrix2x2 }
	 {}
		MatrixType getType() { return type; };
		int getMatrixSize() { return m_22.size() * m_22.at(0).size(); }
		std::array<float, 2> getRowM(int m) { return m_22.at(m); }
		void setMatrixData(int rowNumber, row2x2 rowData) {
			m_22.at(rowNumber) =
			{ rowData.at(0),rowData.at(1) };
		};
		float getElement(int m, int n) { return m_22.at(m).at(n); }
	private:
		const MatrixType type{  };
		std::array<std::array<float, 2>, 2> m_22{};
	};

	struct Matrix3x3 {
		Matrix3x3(row3x3 m0, row3x3 m1, row3x3 m2) : m_33{ {
			{m0.at(0),m0.at(1), m0.at(2)},
			{m1.at(0),m1.at(1), m1.at(2)},
			{m2.at(0),m2.at(1), m2.at(2)}
			}
		}, type{ MatrixType::Matrix3x3 } {}
		MatrixType getType() { return type; };
		int getMatrixSize() { return m_33.size() * m_33.at(0).size(); }
		std::array<float, 3> getRowM(int m) { return m_33.at(m); }
		void setMatrixData(int rowNumber, row3x3 rowData) {
			m_33.at(rowNumber) =
			{ rowData.at(0),rowData.at(1), rowData.at(2) };
		};

		float getElement(int m, int n) { return m_33.at(m).at(n); }

	private:
		const MatrixType type{ };
		std::array<std::array<float, 3>, 3> m_33{};
	};

	struct Matrix4x4 {
		Matrix4x4(row4x4 m0, row4x4 m1, row4x4 m2, row4x4 m3) : m_44{ {
			{m0.at(0),m0.at(1), m0.at(2), m0.at(3)},
			{m1.at(0),m1.at(1), m1.at(2), m1.at(3)},
			{m2.at(0),m2.at(1), m2.at(2), m2.at(3)},
			{m3.at(0),m3.at(1), m3.at(2), m3.at(3)}
			}
		}, type{ MatrixType::Matrix4x4 } {}
		MatrixType getType() { return type; };
		int getMatrixSize() { return m_44.size() * m_44.at(0).size(); }
		std::array<float, 4> getRowM(int m) { return m_44.at(m); }
		std::array<float, 4> getColumn(int n) { return std::array<float, 4>{ {m_44.at(0).at(n), m_44.at(1).at(n), m_44.at(2).at(n), m_44.at(3).at(n)} }; }
		void setMatrixData(int rowNumber, row4x4 rowData) {
			m_44.at(rowNumber) =
			{ rowData.at(0),rowData.at(1), rowData.at(2), rowData.at(3) };
		};
		float getElement(int m, int n) { return m_44.at(m).at(n); }
	private:
		const MatrixType type{ };
		std::array<std::array<float, 4>, 4> m_44{};
	};

	static bool matricesAreEqual_2x2(Matrix2x2 m1, Matrix2x2 m2)
	{
		bool areEqual{ true };
		if (m1.getType() != m2.getType())
		{
			return false;
		}

		for (int m = 0; m < m1.getRowM(0).size(); m++)
		{
			areEqual &= (m1.getRowM(m) == m2.getRowM(m));
		}

		return areEqual;
	}

	static bool matricesAreEqual_3x3(Matrix3x3 m1, Matrix3x3 m2)
	{
		bool areEqual{ true };
		if (m1.getType() != m2.getType())
		{
			return false;
		}

		for (int m = 0; m < m1.getRowM(0).size(); m++)
		{
			areEqual &= (m1.getRowM(m) == m2.getRowM(m));
		}

		return areEqual;
	}

	static bool matricesAreEqual_4x4(Matrix4x4 m1, Matrix4x4 m2)
	{
		bool areEqual{ true };
		if (m1.getType() != m2.getType())
		{
			return false;
		}

		for (int m = 0; m < m1.getRowM(0).size(); m++)
		{
			areEqual &= (m1.getRowM(m) == m2.getRowM(m));
		}

		return areEqual;
	}

	static Matrix4x4 multiplyMatrices(Matrix4x4 m1, Matrix4x4 m2)
	{
		std::array<row4x4, 4> result_row{ {{ 1.0,0.0,0.0,0.0 }, { 0.0,1.0,0.0,0.0 }, { 0.0,0.0,1.0,0.0 }, { 0.0,0.0,0.0,1.0 }} };

		for (int m = 0; m < 4; m++)
		{
			for (int n = 0; n < 4; n++)
			{
				auto [v0_m1, v1_m1, v2_m1, v3_m1] = m1.getRowM(m);
				HomogenousCoordinates row{ v0_m1, v1_m1, v2_m1, v3_m1 };
				auto [v0_m2, v1_m2, v2_m2, v3_m2] = m2.getColumn(n);
				HomogenousCoordinates column{ v0_m2, v1_m2, v2_m2, v3_m2 };
				result_row.at(m).at(n) = dotProduct(row, column);
			}
		}
		Matrix4x4 result{ result_row.at(0), result_row.at(1),result_row.at(2),result_row.at(3) };
		return result;
	}

	static HomogenousCoordinates multiplyMatrixWithTuple(Matrix4x4 m, HomogenousCoordinates c)
	{
		std::array<float, 4> returnValues{};
		for (int row = 0; row < 4; row++)
		{
				auto [v0_m1, v1_m1, v2_m1, v3_m1] = m.getRowM(row);

				returnValues.at(row) = dotProduct(HomogenousCoordinates{ v0_m1, v1_m1, v2_m1, v3_m1 }, c);
			
		}

		return HomogenousCoordinates{ returnValues.at(0),returnValues.at(1),returnValues.at(2),returnValues.at(3) };
	}

	private:
		// helper function to properly compare floats
		static bool floatsAreEqual(float val1, float val2) { constexpr float epsilon{ 0.0001f }; return std::abs(val1 - val2) < epsilon; };
		// point
		HomogenousCoordinates m_p{ 0.0,0.0,0.0,1.0 };
		// vector
		HomogenousCoordinates m_v{ 0.0,0.0,0.0,0.0 };


		
};

