#include <vector>
#include <iostream>
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
					NonZeroRow + 1;
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
						InvertedMatrixPointer->DeterminantMultiple *= -1;
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
		void PopColumn(long long ColumnToRemove = 0)
		{

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
				MBMatrixPartialInvertion<T> PartiallyInvertedMatrix;
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
		MBMatrix<T> MatrixToReturn(NumberOfColumns, NumberOfRows);
		for (size_t CurrentRow = 0; CurrentRow < NumberOfRows; CurrentRow++)
		{
			for (size_t CurrentColumn = 0; CurrentColumn < NumberOfColumns; CurrentColumn++)
			{
				//ij representerar indexet vi ska änra på i den nya matrisen
				T NewValue = 0;
				for (size_t Index = 0; Index < NumberOfColumns; Index++)
				{
					NewValue += Left(CurrentRow, Index) * Right(CurrentColumn, Index);
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