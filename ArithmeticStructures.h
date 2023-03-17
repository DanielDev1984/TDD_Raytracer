#pragma once
#define _USE_MATH_DEFINES
#include <tuple>
#include <array>
#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

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
		float getDeterminant() const { return (m_22.at(0).at(0)*m_22.at(1).at(1)) - (m_22.at(0).at(1)*m_22.at(1).at(0)); };
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

		float getElement(int m, int n)const { return m_33.at(m).at(n); }
		float getDeterminant() const {

			;
			return (getElement(0, 0) * getCofactor(0, 0, *this) + getElement(0, 1) * getCofactor(0, 1, *this) + getElement(0, 2) * getCofactor(0, 2, *this));
		};
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
		float getElement(int m, int n) const { return m_44.at(m).at(n); }
		float getDeterminant() const {
			//todo: fix really stupid (i.e. hardcoded) way of cofactor expansion (e.g.: https://math.libretexts.org/Bookshelves/Linear_Algebra/Interactive_Linear_Algebra_(Margalit_and_Rabinoff)/04%3A_Determinants/4.02%3A_Cofactor_Expansions) implementation by means of e.g. employing a for-loop!
			auto subMatrix0{ getSubmatrixOf4x4Matrix(0,0,*this) };
			auto subMatrix1{ getSubmatrixOf4x4Matrix(0,1,*this) };
			auto subMatrix2{ getSubmatrixOf4x4Matrix(0,2,*this) };
			auto subMatrix3{ getSubmatrixOf4x4Matrix(0,3,*this) };

			auto cofactorSM0{ subMatrix0.getDeterminant() };
			auto cofactorSM1{ -1.0 * subMatrix1.getDeterminant() };
			auto cofactorSM2{ subMatrix2.getDeterminant() };
			auto cofactorSM3{ -1.0 * subMatrix3.getDeterminant() };

			return getElement(0, 0) * cofactorSM0 + 
				getElement(0, 1) * cofactorSM1 + 
				getElement(0, 2) * cofactorSM2 + 
				getElement(0, 3) * cofactorSM3;
		}; 
		bool isInvertible() const { return getDeterminant() != 0; };
		
	private:
		const MatrixType type{ };
		std::array<std::array<float, 4>, 4> m_44{};
	};

	static Matrix4x4 getIdentityMatrix() {
		row4x4 m0{ {1.0, 0.0, 0.0, 0.0} }, m1{ {0.0, 1.0, 0.0, 0.0} }, m2{ {0.0,0.0,1.0, 0.0} }, m3{ {0.0,0.0,0.0,1.0} };
		return Matrix4x4{ m0, m1, m2, m3 };
	}

	static Matrix4x4 transposeMatrix(Matrix4x4& m) {
		std::array<row4x4, 4> columnData;
		for (int column = 0; column < 4; column++)
		{
			columnData.at(column) = m.getColumn(column);
		}
		for (int row = 0; row < 4; row++)
		{
			m.setMatrixData(row, columnData.at(row));
			
		}
		return m;
	}

	static void inverseMatrix(Matrix4x4& m) {
		if(!m.isInvertible())
		{
			std::cout << "------------ matrix not invertible --------------";
				return;
		}
		
		Matrix4x4 m_tmp{ getCofactorMatrix(m) };
		transposeMatrix(m_tmp);
		float const detM{ m.getDeterminant() };
		std::array < row4x4, 4> rowBuffer{};
		for (int row = 0; row < 4; row++)
		{
			for (int column = 0; column < 4; column++)
			{
				rowBuffer.at(row).at(column) = m_tmp.getElement(row, column) / detM;
			}
		}
		for (int row = 0; row < 4; row++)
		{
			m.setMatrixData(row, rowBuffer.at(row));
		}
	
	};
	static Matrix4x4 getCofactorMatrix(Matrix4x4 m) {
		// signs of cofactors
		// | + - + - |
		// | - + - + |
		// | + - + - |
		// | - + - + |
		// row 0
		auto subMatrix0{ getSubmatrixOf4x4Matrix(0,0,m) };
		auto subMatrix1{ getSubmatrixOf4x4Matrix(0,1,m) };
		auto subMatrix2{ getSubmatrixOf4x4Matrix(0,2,m) };
		auto subMatrix3{ getSubmatrixOf4x4Matrix(0,3,m) };

		auto cofactorSM0{ subMatrix0.getDeterminant() };
		auto cofactorSM1{ -1.0 * subMatrix1.getDeterminant() };
		auto cofactorSM2{ subMatrix2.getDeterminant() };
		auto cofactorSM3{ -1.0 * subMatrix3.getDeterminant() };

		row4x4 row0{ cofactorSM0, cofactorSM1, cofactorSM2, cofactorSM3 };

		// row 1
		auto subMatrix0_1{ getSubmatrixOf4x4Matrix(1,0,m) };
		auto subMatrix1_1{ getSubmatrixOf4x4Matrix(1,1,m) };
		auto subMatrix2_1{ getSubmatrixOf4x4Matrix(1,2,m) };
		auto subMatrix3_1{ getSubmatrixOf4x4Matrix(1,3,m) };

		cofactorSM0 = -1.0 *subMatrix0_1.getDeterminant();
		cofactorSM1 =  subMatrix1_1.getDeterminant();
		cofactorSM2 = -1.0 * subMatrix2_1.getDeterminant();
		cofactorSM3 = subMatrix3_1.getDeterminant();

		row4x4 row1{ cofactorSM0, cofactorSM1, cofactorSM2, cofactorSM3 };

		// row 2
		auto subMatrix0_2{ getSubmatrixOf4x4Matrix(2,0,m) };
		auto subMatrix1_2{ getSubmatrixOf4x4Matrix(2,1,m) };
		auto subMatrix2_2{ getSubmatrixOf4x4Matrix(2,2,m) };
		auto subMatrix3_2{ getSubmatrixOf4x4Matrix(2,3,m) };

		cofactorSM0 = subMatrix0_2.getDeterminant();
		cofactorSM1 = -1.0 * subMatrix1_2.getDeterminant();
		cofactorSM2 = subMatrix2_2.getDeterminant();
		cofactorSM3 = -1.0 * subMatrix3_2.getDeterminant();

		row4x4 row2{ cofactorSM0, cofactorSM1, cofactorSM2, cofactorSM3 };

		// row 3
		auto subMatrix0_3{ getSubmatrixOf4x4Matrix(3,0,m) };
		auto subMatrix1_3{ getSubmatrixOf4x4Matrix(3,1,m) };
		auto subMatrix2_3{ getSubmatrixOf4x4Matrix(3,2,m) };
		auto subMatrix3_3{ getSubmatrixOf4x4Matrix(3,3,m) };

		cofactorSM0 = -1.0*subMatrix0_3.getDeterminant();
		cofactorSM1 = subMatrix1_3.getDeterminant();
		cofactorSM2 = -1.0*subMatrix2_3.getDeterminant();
		cofactorSM3 = subMatrix3_3.getDeterminant();

		row4x4 row3{ cofactorSM0, cofactorSM1, cofactorSM2, cofactorSM3 };

		return Matrix4x4{ row0, row1, row2, row3 };
	};

	static Matrix3x3 getSubmatrixOf4x4Matrix(int row, int column, Matrix4x4 m) {
		row3x3 m0{ {1.0, 0.0, 0.0} }, m1{ {0.0, 1.0, 0.0} }, m2{ {0.0,0.0,1.0} };
		std::array<float, 9> subMatrixValues{};
		int i{ 0 };
		for (int local_row = 0; local_row < 4; local_row++)
		{
			if (local_row != row)
			{
				for (int local_column = 0; local_column < 4; local_column++)
				{
					if (local_column != column)
					{
						subMatrixValues.at(i) = m.getElement(local_row, local_column);
						i++;
					}
				}
			}

		}
		return Matrix3x3{ row3x3{subMatrixValues.at(0), subMatrixValues.at(1), subMatrixValues.at(2)},
			row3x3{subMatrixValues.at(3), subMatrixValues.at(4), subMatrixValues.at(5) },
			row3x3{subMatrixValues.at(6), subMatrixValues.at(7), subMatrixValues.at(8)} };

	};

	static Matrix2x2 getSubmatrixOf3x3Matrix(int row, int column, Matrix3x3 m) {
		row2x2 m0{ {1.0, 0.0} }, m1{ {0.0, 1.0} };
		std::array<float, 4> subMatrixValues{};
		int i{ 0 };
		for (int local_row = 0; local_row < 3; local_row++)
		{
			if (local_row != row)
			{
				for (int local_column = 0; local_column < 3; local_column++)
				{
					if (local_column != column)
					{
						subMatrixValues.at(i) = m.getElement(local_row, local_column);
						i++;
					}
				}
			}

		}
		return Matrix2x2{ row2x2{subMatrixValues.at(0), subMatrixValues.at(1)},
			row2x2{subMatrixValues.at(2), subMatrixValues.at(3)}};

	};

	static float  getMinor(int m, int n, const Matrix3x3 matrix) { const auto submatrix{getSubmatrixOf3x3Matrix(m,n,matrix)}; 
	return submatrix.getDeterminant(); };

	static float  getCofactor(int m, int n, const Matrix3x3 matrix) {
		const bool negateMinor{ ((m+n) % 2 != 0) };

		auto minor{ getMinor(m,n, matrix) };

		return negateMinor ? -1.0*minor : minor;
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
			for (int n = 0; n < m1.getColumn(0).size(); n++)
			{

				areEqual &= (floatsAreEqual(m1.getRowM(m).at(n), m2.getRowM(m).at(n)));
			}
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

	static Matrix4x4 getTranslationMatrix(float x, float y, float z)
	{
		row4x4 row0{ 1.0, 0.0, 0.0, x }, row1{ 0.0,1.0,0.0,y }, row2{ 0.0,0.0,1.0,z }, row3{ 0.0,0.0,0.0,1.0 };
		return Matrix4x4{ row0, row1,row2,row3 };
	}

	static Matrix4x4 getScalingMatrix(float scaleX, float scaleY, float scaleZ)
	{
		row4x4 row0{ scaleX, 0.0, 0.0, 0.0 }, row1{ 0.0,scaleY,0.0,0.0 }, row2{ 0.0,0.0,scaleZ,0.0 }, row3{ 0.0,0.0,0.0,1.0 };
		return Matrix4x4{ row0, row1,row2,row3 };
		
	}

	static float getRadiansForDeg(float deg)
	{
		return (deg / 180.0) * M_PI;
	}

	static Matrix4x4 getRotationMatrix_XAxis(float rad)
	{
		return getIdentityMatrix();
	}

	private:
		// helper function to properly compare floats
		static bool floatsAreEqual(float val1, float val2) { constexpr float epsilon{ 0.0001f }; return std::abs(val1 - val2) < epsilon; };
		// point
		HomogenousCoordinates m_p{ 0.0,0.0,0.0,1.0 };
		// vector
		HomogenousCoordinates m_v{ 0.0,0.0,0.0,0.0 };


		
};

