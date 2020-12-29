#pragma once
#include <vector>
#include <iostream>
#include <MBMathTemplateFunctions.h>
#include <math.h>
namespace MBMath
{
	template<typename T> class MBMatrix;

	template<typename T> MBMatrix<T> operator+(const MBMatrix<T>& Left, const MBMatrix<T>& Right);
	template<typename T> MBMatrix<T> operator-(const MBMatrix<T>& Left, const MBMatrix<T>& Right);
	template<typename T> MBMatrix<T> operator*(const MBMatrix<T>& Left, const MBMatrix<T>& Right);
						
	template<typename T> MBMatrix<T> operator*(const MBMatrix<T>& Left, long long Right);
	template<typename T> MBMatrix<T> operator*(long long Left, const MBMatrix<T>& Right);
	template<typename T> MBMatrix<T> operator*(double Left, const MBMatrix<T>& Right);
	template<typename T> MBMatrix<T> operator*(const MBMatrix<T> Left, double Right);
						
	//template<typename T> T& operator[](int Row, int Column);
	template<typename T> class MBMatrixPartialInvertion
	{
	public:
		MBMatrix<T> Matrix;
		int DeterminantMultiple = 1;
		MBMatrixPartialInvertion(long long Size) : Matrix{Size}
		{
		
		}
		MBMatrixPartialInvertion()
		{

		}
	};
	template<typename T>
	class MBMatrix
	{
	private:
		std::vector<std::vector<T>> MatrixData = {};
	public:
		friend MBMatrix<T> operator+<>(const MBMatrix<T>& Left, const MBMatrix<T>& Right);
		friend MBMatrix<T> operator-<>(const MBMatrix<T>& Left, const MBMatrix<T>& Right);
		friend MBMatrix<T> operator*<>(const MBMatrix<T>& Left, const MBMatrix<T>& Right);
									
		friend MBMatrix<T> operator*<>(const MBMatrix<T>& Left, long long Right);
		friend MBMatrix<T> operator*<>(long long Left, const MBMatrix<T>& Right);
		friend MBMatrix<T> operator*<>(double Left, const MBMatrix<T>& Right);
		friend MBMatrix<T> operator*<>(const MBMatrix<T> Left, double Right);

		bool ErrorFlag = false;
		T& operator()(int Row, int Column)
		{
			return(MatrixData[Row][Column]);
		}
		const T&  operator() (int Row, int Column) const
		{
			return(MatrixData[Row][Column]);
		}
		friend std::ostream& operator<<(std::ostream& os,MBMatrix<T> MatrixToPrint)
		{
			int RowCount = MatrixToPrint.NumberOfRows();
			int ColumnCount = MatrixToPrint.NumberOfColumns();
			for (size_t i = 0; i < RowCount; i++)
			{
				for (size_t j = 0; j < ColumnCount; j++)
				{
					std::cout << MatrixToPrint(i, j) << " ";
				}
				std::cout << std::endl;
			}
			return os;
		}
		long long NumberOfRows() const
		{
			return(MatrixData.size());
		}
		long long NumberOfColumns() const
		{
			return(MatrixData[0].size());
		}
		
		void SwitchRows(long long Row1,long long Row2)
		{
			if (Row1 == Row2)
			{
				return;
			}
			else
			{
				std::vector<T> TempRow = MatrixData[Row1];
				long long ColumnCount = NumberOfColumns();
				for (size_t i = 0; i < ColumnCount; i++)
				{
					MatrixData[Row1][i] = MatrixData[Row2][i];
				}
				MatrixData[Row2] = TempRow;
			}
		}
		void SubtractRow(long long RowToSubtractWith, long long RowToSubtract, T MultipleToSubtractWith)
		{
			long long ColumnCount = NumberOfColumns();
			for (size_t i = 0; i < ColumnCount; i++)
			{
				MatrixData[RowToSubtract][i] -= MatrixData[RowToSubtractWith][i] * MultipleToSubtractWith;
			}
		}
		//creates a unit matrix with given side length
		MBMatrix(long long RowCount)
		{
			MatrixData = std::vector<std::vector<T>>(RowCount);
			for (size_t i = 0; i < RowCount; i++)
			{
				MatrixData[i] = std::vector<T>(RowCount);
				for (size_t j = 0; j < RowCount; j++)
				{
					MatrixData[i][j] = 0;
				}
				MatrixData[i][i] = 1;
			}
		}
		MBMatrix<T> GausEliminate(MBMatrixPartialInvertion<T>* InvertedMatrixPointer = nullptr)
		{
			MBMatrix<T> MatrixToReturn = (*this);
			long long RowCount = NumberOfRows();
			//MBMatrix<T> UnitMatrixToTransform;
			//if (InvertedMatrixPointer != nullptr)
			//{
			//	UnitMatrixToTransform = MBMatrix<T>(RowCount);
			//}
			for (size_t i = 0; i < RowCount; i++)
			{
				//letar fram den första raden med ett tal skilt från 0 i denna position, och tar denna till den nuvarande positionen
				int NonZeroRow = i;
				while (NonZeroRow < RowCount)
				{
					if(MatrixToReturn.MatrixData[NonZeroRow][i] != 0)
					{
						break;
					}
					NonZeroRow += 1;
				}
				if (NonZeroRow == RowCount)
				{
					//ingen rad hade icke negativt värde, det kommer inte bli en komplett gauss elimination
					continue;
				}
				else
				{
					//vi eventeullt flyttar radan till nuvarande index, och sedan så subtraherar vi ner allt
					MatrixToReturn.SwitchRows(i, NonZeroRow);
					if (InvertedMatrixPointer != nullptr)
					{
						InvertedMatrixPointer->Matrix.SwitchRows(i, NonZeroRow);
						if (i != NonZeroRow)
						{
							InvertedMatrixPointer->DeterminantMultiple *= -1;
						}
					}
					T SubtractionMultiple = 0;
					for (size_t j = i+1; j < RowCount; j++)
					{
						//TODO tänk ut huruvida multipeln borde vara av samma typ och framförallt om jag bore göra en int specialisering av matrisen?
						//TODO lägger inte till om den är 0, borde man göra det?
						SubtractionMultiple = MatrixToReturn.MatrixData[j][i] / MatrixToReturn.MatrixData[i][i];
						MatrixToReturn.SubtractRow(i, j, SubtractionMultiple);
						if (InvertedMatrixPointer != nullptr)
						{
							InvertedMatrixPointer->Matrix.SubtractRow(i, j, SubtractionMultiple);
						}
					}
				}
			}
			return(MatrixToReturn);
		}
		void PopColumn(long long ColumnToRemove)
		{
			long long RowCount = NumberOfRows();
			for (size_t i = 0; i < RowCount; i++)
			{
				MatrixData[i].erase(MatrixData[i].begin() + ColumnToRemove);
			}
		}
		void PopRow(long long RowToRemove)
		{
			MatrixData.erase(MatrixData.begin() + RowToRemove);
		}
		T Determinant()
		{
			//använder lua factorisering för att få fram determinanten på ett effektivt sätt 
			if (NumberOfColumns() != NumberOfRows())
			{
				//quit
			}
			else
			{
				MBMatrixPartialInvertion<T> PartiallyInvertedMatrix(NumberOfColumns());
				MBMatrix<T> GaussEliminatedMatrix = GausEliminate(&PartiallyInvertedMatrix);
				T DeterminantToReturn = 1;
				long long RowCount = NumberOfRows();
				for (size_t i = 0; i < RowCount; i++)
				{
					DeterminantToReturn *= GaussEliminatedMatrix(i, i);
				}
				return(DeterminantToReturn*PartiallyInvertedMatrix.DeterminantMultiple);
			}
		}
		MBMatrix<T> InvertedMatrix()
		{
			long long ColumnCount = NumberOfColumns();
			long long RowCount = NumberOfRows();
			MBMatrix<T> MatrixToReturn;
			if (ColumnCount != RowCount)
			{
				//error, vi returnar matrixen med en error code
				MatrixToReturn.ErrorFlag = true;
				return(MatrixToReturn);
			}
			MBMatrixPartialInvertion<T> PartiallyInvertedMatrix;
			PartiallyInvertedMatrix.Matrix = MBMatrix<T>(RowCount);
			MatrixToReturn = GausEliminate(&PartiallyInvertedMatrix);
			T MatrixDeterminant = 1;
			for (size_t i = 0; i < RowCount; i++)
			{
				MatrixDeterminant *= MatrixToReturn(i,i);
			}
			if (MatrixDeterminant == 0)
			{
				//error, vi returnar matrixen med en error code
				MatrixToReturn.ErrorFlag = true;
				return(MatrixToReturn);
			}
			else
			{
				MatrixToReturn.FullyReduce(&PartiallyInvertedMatrix);
				return(PartiallyInvertedMatrix.Matrix);
			}
			//nu gör vi nästa steg i eliminationen 
		}
		void AppendColumn(MBMatrix<T> ColumnValues)
		{
			if (ColumnValues.NumberOfColumns() != NumberOfRows())
			{
				//error
			}
			else
			{
				long long RowCount = NumberOfRows();
				for (size_t i = 0; i < RowCount; i++)
				{
					MatrixData[i].push_back(ColumnValues(0, i));
				}
			}
		}
		void DivideRow(long long RowToDivide, T ScalarToDivideWith)
		{
			long long ColumnCount = NumberOfColumns();
			for (size_t i = 0; i < ColumnCount; i++)
			{
				MatrixData[RowToDivide][i] /= ScalarToDivideWith;
			}
		}
		void MultiplyRow(long long RowToMultiply, T ScalarToMultiplyWith)
		{
			long long ColumnCount = NumberOfColumns();
			for (size_t i = 0; i < ColumnCount; i++)
			{
				MatrixData[RowToMultiply][i] *= ScalarToMultiplyWith;
			}
		}
		//solves the equationsystem the matrix system encodes with initial variable values given by the argument as a row matrix 
		///*
		void FullyReduce(MBMatrixPartialInvertion<T>* PartiallyInvertedMatrix = nullptr)
		{
			//vi gör det andra steget av algoritmen och gör en ful gauss eliminering, och om vi vill spara värdena medans vi gör det så ändrar vi partiall matrisen
			long long RowCount = NumberOfRows();
			for (size_t i = 0; i < RowCount; i++)
			{
				if (MatrixData[i][i] == 0)
				{
					continue;
				}
				else
				{
					T NumberToDivideWith = MatrixData[i][i];
					DivideRow(i, NumberToDivideWith);
					if (PartiallyInvertedMatrix != nullptr)
					{
						PartiallyInvertedMatrix->Matrix.DivideRow(i, NumberToDivideWith);
					}
					for (int j = i-1; j >= 0; j--)
					{
						//går uppåt genom raderna och tar bort så att inga har element på samma rad
						if (MatrixData[j][i] != 0)
						{
							T SubtractMultiple = MatrixData[j][i];
							SubtractRow(i, j, SubtractMultiple);
							if (PartiallyInvertedMatrix != nullptr)
							{
								PartiallyInvertedMatrix->Matrix.SubtractRow(i, j, SubtractMultiple);
							}
						}
					}
				}
			}
		}
		MBMatrix<T> SolveMatrix(MBMatrix<T> VariableValues)
		{
			
			MBMatrix<T> DataToReturn(1, VariableValues.NumberOfColumns());
			if (VariableValues.NumberOfColumns() != NumberOfRows() && VariableValues.NumberOfColumns() != 0)
			{
				//error
				DataToReturn.ErrorFlag = true;
				return(DataToReturn);
			}
			else
			{
				MBMatrix<T> GaussEliminatedMatrix = (*this);
				long long RowCount = NumberOfRows();
				long long ColumnCount = NumberOfColumns();
				GaussEliminatedMatrix.AppendColumn(VariableValues);
				GaussEliminatedMatrix = GausEliminate();
				GaussEliminatedMatrix.FullyReduce();
				//vi gör det andra steget av algoritmen och gör en ful gauss eliminering, och om vi vill spara värdena medans vi gör det så ändrar vi partiall matrisen
				for (size_t i = 0; i < RowCount; i++)
				{
					DataToReturn(0, i) = GaussEliminatedMatrix[i][ColumnCount];
				}
				return(DataToReturn);
			}
		}
		//*/

		MBMatrix<T> Transpose()
		{
			MBMatrix<T> MatrixToReturn(NumberOfColumns(), NumberOfRows());
			long long ColumnCount = NumberOfColumns();
			long long RowCount = NumberOfRows();
			for (size_t i = 0; i < ColumnCount; i++)
			{
				for (size_t j = 0; j < RowCount; j++)
				{
					MatrixToReturn(i, j) = (*this)(j, i);
				}
			}
			return(MatrixToReturn);
		}
		MBMatrix()
		{

		}
		MBMatrix(long long NumberOfRows, long long ColumnNumber)
		{
			MatrixData = std::vector<std::vector<T>>(NumberOfRows);
			for (size_t i = 0; i < MatrixData.size(); i++)
			{
				MatrixData[i] = std::vector<T>(ColumnNumber);
			}
		}
		~MBMatrix()
		{

		}
	};
	//klass för att kunna representera transformationer i 3D på ett smidigt sätt
	template<typename T>
	class MBVector
	{
	protected:
		MBMatrix<T> VectorData = {};
	public:
		operator MBMatrix<T>()
		{
			return(VectorData);
		}
		MBVector<T> operator+(MBVector<T> RightVector)
		{
			int RightColumns = RightVector.VectorData.NumberOfColumns();
			int LeftColumns = this->VectorData.NumberOfColumns();
			int MaxColumns;
			if (RightColumns > LeftColumns)
			{
				MaxColumns = RightColumns;
			}
			else
			{
				MaxColumns = LeftColumns;
			}
			MBVector<float> ReturnValue = MBVector<float>(MaxColumns);
			for (size_t i = 0; i < RightColumns; i++)
			{
				ReturnValue[i] += RightVector[i];
			}
			for (size_t i = 0; i < LeftColumns; i++)
			{
				ReturnValue[i] += (*this)[i];
			}
			return(ReturnValue);
		}
		friend std::ostream& operator<<(std::ostream& os, MBVector<T> VectorToPrint)
		{			
			return (os<<VectorToPrint.VectorData);
		}
		friend MBVector<T> operator*(MBMatrix<T> LeftMatrix, MBVector<T> RightVector)
		{
			if ((LeftMatrix.NumberOfColumns()!= LeftMatrix.NumberOfRows()) || (LeftMatrix.NumberOfColumns() != RightVector.VectorData.NumberOfColumns()))
			{
				//error, bli ledsen
			}
			else
			{
				RightVector.VectorData = (LeftMatrix * RightVector.VectorData.Transpose()).Transpose();
				return(RightVector);
			}
		}
		friend MBVector<T> operator*(MBVector<T> LeftVector, MBMatrix<T> RightMatrix)
		{
			if ((RightMatrix.NumberOfColumns() != RightMatrix.NumberOfRows()) || (RightMatrix.NumberOfColumns() != LeftVector.VectorData.NumberOfColumns()))
			{
				//error, bli ledsen
			}
			else
			{
				LeftVector.VectorData = LeftVector.VectorData*RightMatrix;
				return(LeftVector);
			}
		}
		T& operator[](long long Index)
		{
			return(VectorData(0, Index));
		}
		friend	MBVector<T> operator*(MBVector<T> LeftVector, T RightScalar)
		{
			int ColumnCount = LeftVector.VectorData.NumberOfColumns();
			MBVector<T> ReturnValue = MBVector<T>(ColumnCount);
			for (size_t i = 0; i < ColumnCount; i++)
			{
				ReturnValue[i] = LeftVector[i] * RightScalar;
			}
			return(ReturnValue);
		}
		friend	MBVector<T> operator*(T LeftScalar, MBVector<T> RightVector)
		{
			int ColumnCount = RightVector.VectorData.NumberOfColumns();
			MBVector<T> ReturnValue = MBVector<T>(ColumnCount);
			for (size_t i = 0; i < ColumnCount; i++)
			{
				ReturnValue[i] = RightVector[i] * LeftScalar;
			}
			return(ReturnValue);
		}
		MBVector(long long NumberOfDimensions) : VectorData(1, NumberOfDimensions)
		{
			for (size_t i = 0; i < NumberOfDimensions; i++)
			{
				VectorData(0, i) = 0;
			}
		}
		MBVector()
		{

		}
		MBVector(int NumberOfElements)
		{
			VectorData = MBMatrix<T>(1, NumberOfElements);
		}
		//eftersom vi vill kunna rotera alla typer av koordinater men inte nödvändigtvis returna samma typ av värden om vi 
		//TODO Gör en generell rotationsmatrix funktion, just nu har jag däremot inte riktigt matten för det
		//void Rotate(double AngleToRotate, MBVector<T> AxisToRotateFrom)
		//{
		MBVector Normalized()
		{
			long long ColumnCount = VectorData.NumberOfColumns();
			MBVector<T> ReturnValue(ColumnCount);
			T NormOfVector = 0;
			//TODO för att faktiskt få en helt generell metod för att räkna avstånd behöver vi en generell abs funktion och en generell rotfunktion
			for (size_t i = 0; i < ColumnCount; i++)
			{
				NormOfVector += VectorData(0, i) * VectorData(0, i);
			}
			if (NormOfVector != 0)
			{
				NormOfVector = MBMath::Sqrt<T>(NormOfVector);
				for (size_t i = 0; i < ColumnCount; i++)
				{
					ReturnValue[i] = VectorData(0, i) / NormOfVector;
				}
				return(ReturnValue);
			}
			else
			{
				//egentligen ger detta ett error, noll vektorn kan ju inte normaliseras
				//men vi tar och barar returnar värdet
				return((*this));
			}
		}
		T DotProduct(MBVector RightVector)
		{
			T ReturnValue = 0;
			long long ColumnCount = VectorData.NumberOfColumns();
			for (size_t i = 0; i < ColumnCount; i++)
			{
				ReturnValue += VectorData(0, i) * RightVector.VectorData(0, i);
			}
			return(ReturnValue);
		}
		//}
	};
	template<typename T> class MBVector3 : public MBVector<T>
	{
	public:
		MBVector3<T> CrossProduct(MBVector3 RightVector)
		{
			MBVector3 ReturnValue(0,0,0);
			MBMatrix<T> TempMatrix(2);
			TempMatrix(0, 0) = this->VectorData(0, 1);
			TempMatrix(1, 0) = this->VectorData(0, 2);
			TempMatrix(0, 1) = RightVector.VectorData(0, 1);
			TempMatrix(1, 1) = RightVector.VectorData(0, 2);
			ReturnValue[0] = TempMatrix.Determinant();
			TempMatrix(0, 0) = this->VectorData(0, 0);
			TempMatrix(1, 0) = this->VectorData(0, 2);
			TempMatrix(0, 1) = RightVector.VectorData(0, 0);
			TempMatrix(1, 1) = RightVector.VectorData(0, 2);
			ReturnValue[1] = -TempMatrix.Determinant();
			TempMatrix(0, 0) = this->VectorData(0, 0);
			TempMatrix(1, 0) = this->VectorData(0, 1);
			TempMatrix(0, 1) = RightVector.VectorData(0, 0);
			TempMatrix(1, 1) = RightVector.VectorData(0, 1);
			ReturnValue[2] = TempMatrix.Determinant();
			return(ReturnValue);
		}
		MBVector3 ZAxis()
		{
			return(MBVector3(0, 0, 1));
		}
		MBVector3() : MBVector<T>(3)
		{
			this->VectorData = MBMatrix<T>(1, 3);
		}
		static MBMatrix<T> GetRotationMatrix(double AngleToRotate, MBVector<T> AxisToRotateFrom)
		{
			AngleToRotate *= (double)3.141592653589793238 / 180;
			AxisToRotateFrom = AxisToRotateFrom.Normalized();
			long long ColumnCount = 3;
			MBVector3 e1;
			bool e1Finished = false;
			for (size_t i = 0; i < ColumnCount; i++)
			{
				if (AxisToRotateFrom[i] == 0)
				{
					e1[i] = 1;
					e1Finished = true;
					break;
				}
			}
			if (!e1Finished)
			{
				e1[0] = -AxisToRotateFrom[1];
				e1[1] = AxisToRotateFrom[0];
			}
			e1 = e1.Normalized();
			MBVector3<T> e2 = MBVector3(AxisToRotateFrom).CrossProduct(e1);
			//T Test = e1.DotProduct(AxisToRotateFrom);
			//T Test2 = e1.DotProduct(e2);
			//T Test3 = e2.DotProduct(AxisToRotateFrom);
			//nu vill vi konstruera e1 i vår nya bas, den har enbart egenskapen att dens skalar produkt med vektor ar 0.
			MBMatrix<T> BaseMatrix(3);
			BaseMatrix(0, 0) = e1[0];
			BaseMatrix(1, 0) = e1[1];
			BaseMatrix(2, 0) = e1[2];
			BaseMatrix(0, 2) = AxisToRotateFrom[0];
			BaseMatrix(1, 2) = AxisToRotateFrom[1];
			BaseMatrix(2, 2) = AxisToRotateFrom[2];
			BaseMatrix(0, 1) = e2[0];
			BaseMatrix(1, 1) = e2[1];
			BaseMatrix(2, 1) = e2[2];
			MBMatrix<T> RegularRotation(3);
			RegularRotation(0, 0) = std::cos(AngleToRotate);
			RegularRotation(1, 1) = std::cos(AngleToRotate);
			RegularRotation(0, 1) = -std::sin(AngleToRotate);
			RegularRotation(1, 0) = std::sin(AngleToRotate);
			//std::cout << BaseMatrix << std::endl;
			//std::cout << BaseMatrix.Transpose() << std::endl;
			//std::cout << RegularRotation << std::endl;
			MBMatrix<T> RotationMatrix = BaseMatrix * RegularRotation * BaseMatrix.Transpose();
			return(RotationMatrix);
		}
		void Rotate(double AngleToRotate, MBVector<T> AxisToRotateFrom)
		{
			AngleToRotate *= (double)3.141592653589793238 / 180;
			AxisToRotateFrom = AxisToRotateFrom.Normalized();
			long long ColumnCount = this->VectorData.NumberOfColumns();
			MBVector3 e1;
			bool e1Finished = false;
			for (size_t i = 0; i < ColumnCount; i++)
			{
				if(AxisToRotateFrom[i] == 0)
				{
					e1[i] = 1;
					e1Finished = true;
					break;
				}
			}
			if (!e1Finished)
			{
				e1[0] = -AxisToRotateFrom[1];
				e1[1] = AxisToRotateFrom[0];
			}
			e1 = e1.Normalized();
			MBVector3<T> e2 = MBVector3(AxisToRotateFrom).CrossProduct(e1);
			//T Test = e1.DotProduct(AxisToRotateFrom);
			//T Test2 = e1.DotProduct(e2);
			//T Test3 = e2.DotProduct(AxisToRotateFrom);
			//nu vill vi konstruera e1 i vår nya bas, den har enbart egenskapen att dens skalar produkt med vektor ar 0.
			MBMatrix<T> BaseMatrix(3);
			BaseMatrix(0, 0) = e1[0];
			BaseMatrix(1, 0) = e1[1];
			BaseMatrix(2, 0) = e1[2];
			BaseMatrix(0, 2) = AxisToRotateFrom[0];
			BaseMatrix(1, 2) = AxisToRotateFrom[1];
			BaseMatrix(2, 2) = AxisToRotateFrom[2];
			BaseMatrix(0, 1) = e2[0];
			BaseMatrix(1, 1) = e2[1];
			BaseMatrix(2, 1) = e2[2];
			MBMatrix<T> RegularRotation(3);
			RegularRotation(0, 0) = std::cos(AngleToRotate);
			RegularRotation(1, 1) = std::cos(AngleToRotate);
			RegularRotation(0, 1) = -std::sin(AngleToRotate);
			RegularRotation(1, 0) = std::sin(AngleToRotate);
			//std::cout << BaseMatrix << std::endl;
			//std::cout << BaseMatrix.Transpose() << std::endl;
			//std::cout << RegularRotation << std::endl;
			MBMatrix<T> RotationMatrix = BaseMatrix * RegularRotation * BaseMatrix.Transpose();
			//std::cout << RotationMatrix << std::endl;
			this->VectorData = (RotationMatrix * this->VectorData.Transpose()).Transpose();
		}
		MBVector3(T x , T y, T z)
		{
			this->VectorData = MBMatrix<T>(1, 3);
			this->VectorData(0, 0) = x;
			this->VectorData(0, 1) = y;
			this->VectorData(0, 2) = z;
		}
		MBVector3(MBMatrix<T> MatrixToConvert)
		{
			if (MatrixToConvert.NumberOfColumns() == 3)
			{
				this->VectorData(0, 0) = MatrixToConvert(0, 0);
				this->VectorData(0, 1) = MatrixToConvert(0, 1);
				this->VectorData(0, 2) = MatrixToConvert(0, 2);
			}
			else if(MatrixToConvert.NumberOfRows() == 3)
			{
				this->VectorData(0, 0) = MatrixToConvert(0, 0);
				this->VectorData(0, 1) = MatrixToConvert(1, 0);
				this->VectorData(0, 2) = MatrixToConvert(2, 0);
			}
			else
			{
				//error handle eller default, vem vet
			}
		}
		MBVector3(MBVector<T> VectorToConvert) : MBVector<T>(3)
		{
			(*this)[0] = VectorToConvert[0];
			(*this)[1] = VectorToConvert[1];
			(*this)[2] = VectorToConvert[2];
		}
	};
	template<typename T>
	class MBMatrix4
	{
	private:
		MBMatrix<T> MatrixData = MBMatrix<T>(4);
		std::vector<T> ContinousData = std::vector<T>(16);
	public:
		void PrintWolframMatrix()
		{
			std::cout <<"{";
			for (size_t i = 0; i < 4; i++)
			{
				std::cout << "{";
				for (size_t j = 0; j < 3; j++)
				{
					std::cout << (*this)(i, j)<<",";
				}
				std::cout << (*this)(i, 3) << "},";
			}
			std::cout << "}";
			std::cout<<std::endl;
		}
		friend std::ostream& operator<<(std::ostream& os, MBMatrix4<T> MatrixToPrint)
		{
			return (os << MatrixToPrint.MatrixData);
		}
		T& operator() (int RowIndex, int ColumnIndex)
		{
			return(MatrixData(RowIndex, ColumnIndex));
		}
		const T& operator() (int Row, int Column) const
		{
			return(MatrixData(Row,Column));
		}
		T& operator[](int DiagonalIndex)
		{
			return(MatrixData(DiagonalIndex, DiagonalIndex));
		}
		friend MBMatrix4<T> operator*(const MBMatrix4<T>& Left, const MBMatrix4<T>& Right)
		{
			//nu gör vi matris multiplikation;
			long long NumberOfRows = 4;
			long long NumberOfColumns = 4;
			long long InnerIterations = 4;
			MBMatrix4<T> MatrixToReturn = MBMatrix4<T>();
			for (size_t CurrentRow = 0; CurrentRow < NumberOfRows; CurrentRow++)
			{
				for (size_t CurrentColumn = 0; CurrentColumn < NumberOfColumns; CurrentColumn++)
				{
					//ij representerar indexet vi ska änra på i den nya matrisen
					T NewValue = 0;
					for (size_t Index = 0; Index < InnerIterations; Index++)
					{
						NewValue += Left(CurrentRow, Index) * Right(Index, CurrentColumn);
					}
					MatrixToReturn(CurrentRow, CurrentColumn) = NewValue;
				}
			}
			return(MatrixToReturn);
		}
		friend MBVector3<T> operator*(MBMatrix4<T> LeftMatrix, MBVector3<T> RightVector)
		{
			MBMatrix4 NewRightMatrix(RightVector);
			NewRightMatrix = LeftMatrix * NewRightMatrix;
			return(MBVector3(NewRightMatrix[0], NewRightMatrix[1], NewRightMatrix[2]));
		}
		T* GetContinousDataPointer()
		{
			//uppdaterar datan i den
			//TODO optimisera så att den enbart accessar datan när den optimerar eller specialisera den änn mer så att vi sparar datan på det sättet direkt
			//Row Major Order
			for (size_t i = 0; i < 4; i++)
			{
				for (size_t j = 0; j < 4; j++)
				{
					ContinousData[i * 4 + j] = (*this)(i, j);
				}
			}
			return(&ContinousData[0]);
		}
		MBMatrix4(MBVector3<T> Vector)
		{
			(*this)[0] = Vector[0];
			(*this)[1] = Vector[1];
			(*this)[2] = Vector[2];
			(*this)[3] = 1;
		}
		MBMatrix4() {};
		MBMatrix4(MBMatrix<T> MatrixToCopy)
		{
			int RowCount = MatrixToCopy.NumberOfRows();
			int ColumnCount = MatrixToCopy.NumberOfColumns();
			if (!(RowCount > 4 || ColumnCount > 4))
			{
				for (size_t i = 0; i < RowCount; i++)
				{
					for (size_t j = 0; j < ColumnCount; j++)
					{
						(*this)(i, j) = MatrixToCopy(i, j);
					}
				}
			}
			else
			{
				//error, bli ledsen
			}
		}
	};
	template<typename T>
	class Base
	{
	private:
		MBMath::MBMatrix<T> BaseData;
	public:
		T& operator[](long long Index)
		{
			return(BaseData	(0, Index));
		}
		MBMatrix<T> GetBaseChangeMatrix(Base BaseToChangeTo)
		{

		}
		MBVector<T> GetVectorCoordinates(MBVector<T> VectorDoGetCoordinatesFrom)
		{

		}
	};

};
//MBMath::MBMatrix<float> Test();
//början på cpp filen
namespace MBMath
{
	template<typename T> MBMatrix<T> operator+(const MBMatrix<T>& Left, const MBMatrix<T>& Right)
	{
		MBMatrix<T> MatrixToReturn(Left.MatrixData.size(),Left.MatrixData[0].size());
		for (size_t i = 0; i < Left.MatrixData.size(); i++)
		{
			for (size_t j = 0; j < Left.MatrixData[0].size(); j++)
			{
				MatrixToReturn.MatrixData[i][j] = Left[i, j] + Right[i, j];
			}
		}
	}
	template<typename T> MBMatrix<T> operator-(const MBMatrix<T>& Left, const MBMatrix<T>& Right)
	{
		MBMatrix<T> MatrixToReturn(Left.MatrixData.size(), Left.MatrixData[0].size());
		for (size_t i = 0; i < Left.MatrixData.size(); i++)
		{
			for (size_t j = 0; j < Left.MatrixData[0].size(); j++)
			{
				MatrixToReturn.MatrixData[i][j] = Left[i, j] - Right[i, j];
			}
		}
	}
	template<typename T> MBMatrix<T> operator*(const MBMatrix<T>& Left, const MBMatrix<T>& Right)
	{
		//nu gör vi matris multiplikation;
		long long NumberOfRows = Left.NumberOfRows();
		long long NumberOfColumns = Right.NumberOfColumns();
		long long InnerIterations;
		if (NumberOfColumns > NumberOfRows)
		{
			InnerIterations = NumberOfColumns;
		}
		else
		{
			InnerIterations = NumberOfRows;
		}
		MBMatrix<T> MatrixToReturn(NumberOfRows, NumberOfColumns);
		for (size_t CurrentRow = 0; CurrentRow < NumberOfRows; CurrentRow++)
		{
			for (size_t CurrentColumn = 0; CurrentColumn < NumberOfColumns; CurrentColumn++)
			{
				//ij representerar indexet vi ska änra på i den nya matrisen
				T NewValue = 0;
				for (size_t Index = 0; Index < InnerIterations; Index++)
				{
					NewValue += Left(CurrentRow, Index) * Right(Index, CurrentColumn);
				}
				MatrixToReturn(CurrentRow, CurrentColumn) = NewValue;
			}
		}
		return(MatrixToReturn);
	}
									 
	template<typename T> 
	MBMatrix<T> operator*(const MBMatrix<T>& Left, long long Right)
	{
		long long NumberOfRows = Left.NumberOfRows();
		long long NumberOfColumns = Left.NumberOfColumns();
		MBMatrix<T> MatrixToReturn(NumberOfRows,NumberOfColumns);
		for (size_t i = 0; i < NumberOfRows; i++)
		{
			for (size_t j = 0; j < NumberOfColumns; j++)
			{
				MatrixToReturn(i, j) = Left(i,j)*Right;
			}
		}
		return(MatrixToReturn);
	}
	template<typename T> MBMatrix<T> operator*(long long Left, const MBMatrix<T>& Right)
	{
		long long NumberOfRows = Right.NumberOfRows();
		long long NumberOfColumns = Right.NumberOfColumns();
		for (size_t i = 0; i < NumberOfRows; i++)
		{
			for (size_t j = 0; j < NumberOfColumns; j++)
			{
				Right(i, j) *= Left;
			}
		}
	}
	template<typename T> MBMatrix<T> operator*(double Left, const MBMatrix<T>& Right)
	{
		long long NumberOfRows = Right.NumberOfRows();
		long long NumberOfColumns = Right.NumberOfColumns();
		for (size_t i = 0; i < NumberOfRows; i++)
		{
			for (size_t j = 0; j < NumberOfColumns; j++)
			{
				Right(i, j) *= Left;
			}
		}
	}
	template<typename T> MBMatrix<T> operator*(const MBMatrix<T> Left, double Right)
	{
		long long NumberOfRows = Left.NumberOfRows();
		long long NumberOfColumns = Left.NumberOfColumns();
		for (size_t i = 0; i < NumberOfRows; i++)
		{
			for (size_t j = 0; j < NumberOfColumns; j++)
			{
				Left(i, j) *= Right;
			}
		}
	}
}

//#include<MBMatrix.cpp>