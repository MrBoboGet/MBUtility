#pragma once
#include <vector>
#include <iostream>
#include "MBMathTemplateFunctions.h"
#include <math.h>
#include <cstdint>
#include <exception>
#include <ostream>
#include "MBVector.h"
namespace MBMath
{
    template<typename T> class MBDynamicMatrix;

    template<typename T> MBDynamicMatrix<T> operator+(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right);
    template<typename T> MBDynamicMatrix<T> operator-(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right);
    template<typename T> MBDynamicMatrix<T> operator*(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right);
                        
    template<typename T> MBDynamicMatrix<T> operator*(const MBDynamicMatrix<T>& Left, long long Right);
    template<typename T> MBDynamicMatrix<T> operator*(long long Left, const MBDynamicMatrix<T>& Right);
    template<typename T> MBDynamicMatrix<T> operator*(double Left, const MBDynamicMatrix<T>& Right);
    template<typename T> MBDynamicMatrix<T> operator*(const MBDynamicMatrix<T> Left, double Right);
                        
    //template<typename T> T& operator[](int Row, int Column);
    template<typename T> class MBMatrixPartialInvertion
    {
    public:
        MBDynamicMatrix<T> Matrix;
        int DeterminantMultiple = 1;
        MBMatrixPartialInvertion(long long Size) : Matrix{Size}
        {
        
        }
        MBMatrixPartialInvertion()
        {

        }
    };
    template<typename T>
    class MBDynamicMatrix
    {
    private:
        int m_ColumnCount = 0;
        int m_RowCount = 0;
        MBUtility::MBVector<T,0> MatrixData = {};
    public:
        friend MBDynamicMatrix<T> operator+<>(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right);
        friend MBDynamicMatrix<T> operator-<>(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right);
        friend MBDynamicMatrix<T> operator*<>(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right);
                                    
        friend MBDynamicMatrix<T> operator*<>(const MBDynamicMatrix<T>& Left, long long Right);
        friend MBDynamicMatrix<T> operator*<>(long long Left, const MBDynamicMatrix<T>& Right);
        friend MBDynamicMatrix<T> operator*<>(double Left, const MBDynamicMatrix<T>& Right);
        friend MBDynamicMatrix<T> operator*<>(const MBDynamicMatrix<T> Left, double Right);

        T& operator()(int Row, int Column)
        {
            //return(MatrixData[Row][Column]);
            return(MatrixData[(Row*m_ColumnCount)+Column]);
        }
        const T&  operator() (int Row, int Column) const
        {
            return(MatrixData[(Row*m_ColumnCount)+Column]);
        }
        friend std::ostream& operator<<(std::ostream& os,MBDynamicMatrix<T> MatrixToPrint)
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
            return(m_RowCount);
        }
        long long NumberOfColumns() const
        {
            return(m_ColumnCount);
        }
        
        void SwitchRows(long long Row1,long long Row2)
        {
            if (Row1 == Row2)
            {
                return;
            }
            else
            {
                //std::vector<T> TempRow = MatrixData[Row1];
                MBUtility::MBVector<T,0> TempRow(NumberOfColumns());
                long long ColumnCount = NumberOfColumns();
                for(int i = 0; i < ColumnCount;i++)
                {
                    TempRow[i] = (*this)(Row1,i);   
                }
                for (size_t i = 0; i < ColumnCount; i++)
                {
                    //MatrixData[Row1][i] = MatrixData[Row2][i];
                    (*this)(Row1,i) = (*this)(Row2,i);
                }
                for(int i = 0; i < ColumnCount;i++)
                {
                    (*this)(Row2,i) = TempRow[i];
                }
                //MatrixData[Row2] = TempRow;
            }
        }
        void SubtractRow(long long RowToSubtractWith, long long RowToSubtract, T MultipleToSubtractWith)
        {
            long long ColumnCount = NumberOfColumns();
            for (size_t i = 0; i < ColumnCount; i++)
            {
                (*this)(RowToSubtract,i) -= (*this)(RowToSubtractWith,i) * MultipleToSubtractWith;
            }
        }
        //creates a unit matrix with given side length
        explicit MBDynamicMatrix(long long RowCount)
        {
            m_RowCount = RowCount;
            m_ColumnCount = RowCount;
            MatrixData = MBUtility::MBVector<T,0>(RowCount*RowCount);
            for (size_t i = 0; i < RowCount; i++)
            {
                for (size_t j = 0; j < RowCount; j++)
                {
                    (*this)(i,j) = 0;
                }
                (*this)(i,i) = 1;
            }
        }
        MBDynamicMatrix<T> GausEliminate(MBMatrixPartialInvertion<T>* InvertedMatrixPointer = nullptr)
        {
            MBDynamicMatrix<T> MatrixToReturn = (*this);
            long long RowCount = NumberOfRows();
            //MBMatrix<T> UnitMatrixToTransform;
            //if (InvertedMatrixPointer != nullptr)
            //{
            //    UnitMatrixToTransform = MBMatrix<T>(RowCount);
            //}
            for (size_t i = 0; i < RowCount; i++)
            {
                //letar fram den första raden med ett tal skilt från 0 i denna position, och tar denna till den nuvarande positionen
                int NonZeroRow = i;
                while (NonZeroRow < RowCount)
                {
                    //if(MatrixToReturn.MatrixData[NonZeroRow][i] != 0)
                    if(MatrixToReturn(NonZeroRow,i) != 0)
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
                        //SubtractionMultiple = MatrixToReturn.MatrixData[j][i] / MatrixToReturn.MatrixData[i][i];
                        SubtractionMultiple = MatrixToReturn(j,i) / MatrixToReturn(i,i);
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
        //void PopColumn(long long ColumnToRemove)
        //{
        //    long long RowCount = NumberOfRows();
        //    for (size_t i = 0; i < RowCount; i++)
        //    {
        //        MatrixData[i].erase(MatrixData[i].begin() + ColumnToRemove);
        //    }
        //}
        //void PopRow(long long RowToRemove)
        //{
        //    MatrixData.erase(MatrixData.begin() + RowToRemove);
        //}
        T Determinant()
        {
            //använder lua factorisering för att få fram determinanten på ett effektivt sätt 
            if (NumberOfColumns() != NumberOfRows())
            {
                //quit
                throw std::runtime_error("Can't take the determinant of non square matrix");
            }
            else
            {
                MBMatrixPartialInvertion<T> PartiallyInvertedMatrix(NumberOfColumns());
                MBDynamicMatrix<T> GaussEliminatedMatrix = GausEliminate(&PartiallyInvertedMatrix);
                T DeterminantToReturn = 1;
                long long RowCount = NumberOfRows();
                for (size_t i = 0; i < RowCount; i++)
                {
                    DeterminantToReturn *= GaussEliminatedMatrix(i, i);
                }
                return(DeterminantToReturn*PartiallyInvertedMatrix.DeterminantMultiple);
            }
        }
        MBDynamicMatrix<T> InvertedMatrix()
        {
            long long ColumnCount = NumberOfColumns();
            long long RowCount = NumberOfRows();
            MBDynamicMatrix<T> MatrixToReturn;
            if (ColumnCount != RowCount)
            {
                throw std::runtime_error("Matrix is singular");
                return(MatrixToReturn);
            }
            MBMatrixPartialInvertion<T> PartiallyInvertedMatrix;
            PartiallyInvertedMatrix.Matrix = MBDynamicMatrix<T>(RowCount);
            MatrixToReturn = GausEliminate(&PartiallyInvertedMatrix);
            T MatrixDeterminant = 1;
            for (size_t i = 0; i < RowCount; i++)
            {
                MatrixDeterminant *= MatrixToReturn(i,i);
            }
            if (MatrixDeterminant == 0)
            {
                //error, vi returnar matrixen med en error code
                throw std::runtime_error("Matrix is singular");
                return(MatrixToReturn);
            }
            else
            {
                MatrixToReturn.FullyReduce(&PartiallyInvertedMatrix);
                return(PartiallyInvertedMatrix.Matrix);
            }
            return(MatrixToReturn);
            //nu gör vi nästa steg i eliminationen 
        }
        //void AppendColumn(MBDynamicMatrix<T> ColumnValues)
        //{
        //    if (ColumnValues.NumberOfColumns() != NumberOfRows())
        //    {
        //        //error
        //    }
        //    else
        //    {
        //        long long RowCount = NumberOfRows();
        //        for (size_t i = 0; i < RowCount; i++)
        //        {
        //            MatrixData[i].push_back(ColumnValues(0, i));
        //        }
        //    }
        //}
        void DivideRow(long long RowToDivide, T ScalarToDivideWith)
        {
            long long ColumnCount = NumberOfColumns();
            for (size_t i = 0; i < ColumnCount; i++)
            {
                (*this)(RowToDivide,i) /= ScalarToDivideWith;
                //MatrixData[RowToDivide][i] /= ScalarToDivideWith;
            }
        }
        void MultiplyRow(long long RowToMultiply, T ScalarToMultiplyWith)
        {
            long long ColumnCount = NumberOfColumns();
            for (size_t i = 0; i < ColumnCount; i++)
            {
                (*this)(RowToMultiply,i) *= ScalarToMultiplyWith;
                //MatrixData[RowToMultiply][i] *= ScalarToMultiplyWith;
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
                if ((*this)(i,i) == 0)
                {
                    continue;
                }
                else
                {
                    T NumberToDivideWith = (*this)(i,i);
                    DivideRow(i, NumberToDivideWith);
                    if (PartiallyInvertedMatrix != nullptr)
                    {
                        PartiallyInvertedMatrix->Matrix.DivideRow(i, NumberToDivideWith);
                    }
                    for (int j = i-1; j >= 0; j--)
                    {
                        //går uppåt genom raderna och tar bort så att inga har element på samma rad
                        //if (MatrixData[j][i] != 0)
                        if ((*this)(j,i) != 0)
                        {
                            //T SubtractMultiple = MatrixData[j][i];
                            T SubtractMultiple = (*this)(j,i);
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
        MBDynamicMatrix<T> SolveMatrix(MBDynamicMatrix<T> VariableValues)
        {
            
            MBDynamicMatrix<T> DataToReturn(1, VariableValues.NumberOfColumns());
            if (VariableValues.NumberOfColumns() != NumberOfRows() && VariableValues.NumberOfColumns() != 0)
            {
                throw std::runtime_error("Incompatible dimensions for matrix to solve");
                return(DataToReturn);
            }
            else
            {
                MBDynamicMatrix<T> GaussEliminatedMatrix = (*this);
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

        MBDynamicMatrix<T> Transpose()
        {
            MBDynamicMatrix<T> MatrixToReturn(NumberOfColumns(), NumberOfRows());
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
        MBDynamicMatrix(MBDynamicMatrix const&) = default;
        MBDynamicMatrix(MBDynamicMatrix&&) = default;
        MBDynamicMatrix()
        {

        }
        MBDynamicMatrix(long long NumberOfRows, long long ColumnNumber)
        {
            //MatrixData = std::vector<std::vector<T>>(NumberOfRows);
            m_RowCount = NumberOfRows;
            m_ColumnCount = ColumnNumber;
            MatrixData = MBUtility::MBVector<T,0>(NumberOfRows*ColumnNumber);
        }
        MBDynamicMatrix& operator=(MBDynamicMatrix const&) = default;
        MBDynamicMatrix& operator=(MBDynamicMatrix&&) = default;
        ~MBDynamicMatrix()
        {

        }
    };
    //klass för att kunna representera transformationer i 3D på ett smidigt sätt
    template<typename T>
    class MBDynamicVector
    {
    protected:
        MBDynamicMatrix<T> VectorData = {};
    public:
        operator MBDynamicMatrix<T>()
        {
            return(VectorData);
        }
        MBDynamicVector<T> operator+(MBDynamicVector<T> RightVector)
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
            MBDynamicVector<float> ReturnValue = MBDynamicVector<float>(MaxColumns);
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
        MBDynamicVector<T> operator-(MBDynamicVector<T> RightVector)
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
            MBDynamicVector<float> ReturnValue = MBDynamicVector<float>(MaxColumns);
            for (size_t i = 0; i < RightColumns; i++)
            {
                ReturnValue[i] -= RightVector[i];
            }
            for (size_t i = 0; i < LeftColumns; i++)
            {
                ReturnValue[i] += (*this)[i];
            }
            return(ReturnValue);
        }
        friend std::ostream& operator<<(std::ostream& os, MBDynamicVector<T> VectorToPrint)
        {            
            return (os<<VectorToPrint.VectorData);
        }
        friend MBDynamicVector<T> operator*(MBDynamicMatrix<T> LeftMatrix, MBDynamicVector<T> RightVector)
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
        friend MBDynamicVector<T> operator*(MBDynamicVector<T> LeftVector, MBDynamicMatrix<T> RightMatrix)
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
        T const& operator[](long long Index) const
        {
            return(VectorData(0, Index));
        }
        friend    MBDynamicVector<T> operator*(MBDynamicVector<T> LeftVector, T RightScalar)
        {
            int ColumnCount = LeftVector.VectorData.NumberOfColumns();
            MBDynamicVector<T> ReturnValue = MBDynamicVector<T>(ColumnCount);
            for (size_t i = 0; i < ColumnCount; i++)
            {
                ReturnValue[i] = LeftVector[i] * RightScalar;
            }
            return(ReturnValue);
        }
        friend    MBDynamicVector<T> operator*(T LeftScalar, MBDynamicVector<T> RightVector)
        {
            int ColumnCount = RightVector.VectorData.NumberOfColumns();
            MBDynamicVector<T> ReturnValue = MBDynamicVector<T>(ColumnCount);
            for (size_t i = 0; i < ColumnCount; i++)
            {
                ReturnValue[i] = RightVector[i] * LeftScalar;
            }
            return(ReturnValue);
        }
        MBDynamicVector(long long NumberOfDimensions) : VectorData(1, NumberOfDimensions)
        {
            for (size_t i = 0; i < NumberOfDimensions; i++)
            {
                VectorData(0, i) = 0;
            }
        }
        MBDynamicVector()
        {

        }
        MBDynamicVector(int NumberOfElements)
        {
            VectorData = MBDynamicMatrix<T>(1, NumberOfElements);
        }
        //eftersom vi vill kunna rotera alla typer av koordinater men inte nödvändigtvis returna samma typ av värden om vi 
        //TODO Gör en generell rotationsmatrix funktion, just nu har jag däremot inte riktigt matten för det
        //void Rotate(double AngleToRotate, MBVector<T> AxisToRotateFrom)
        //{
        MBDynamicVector Normalized()
        {
            long long ColumnCount = VectorData.NumberOfColumns();
            MBDynamicVector<T> ReturnValue(ColumnCount);
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
        T DotProduct(MBDynamicVector RightVector)
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
    template<typename T> class MBDynamicVector3 : public MBDynamicVector<T>
    {
    public:
        MBDynamicVector3<T> CrossProduct(MBDynamicVector3 RightVector)
        {
            MBDynamicVector3 ReturnValue(0,0,0);
            MBDynamicMatrix<T> TempMatrix(2);
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
        //MBVector3 ZAxis()
        //{
        //    return(MBVector3(0, 0, 1));
        //}
        MBDynamicVector3() : MBDynamicVector<T>(3)
        {
            this->VectorData = MBDynamicMatrix<T>(1, 3);
        }
        static MBDynamicMatrix<T> GetRotationMatrix(double AngleToRotate, MBDynamicVector<T> AxisToRotateFrom)
        {
            AngleToRotate *= (double)3.141592653589793238 / 180;
            AxisToRotateFrom = AxisToRotateFrom.Normalized();
            long long ColumnCount = 3;
            MBDynamicVector3 e1;
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
            MBDynamicVector3<T> e2 = MBDynamicVector3(AxisToRotateFrom).CrossProduct(e1);
            //T Test = e1.DotProduct(AxisToRotateFrom);
            //T Test2 = e1.DotProduct(e2);
            //T Test3 = e2.DotProduct(AxisToRotateFrom);
            //nu vill vi konstruera e1 i vår nya bas, den har enbart egenskapen att dens skalar produkt med vektor ar 0.
            MBDynamicMatrix<T> BaseMatrix(3);
            BaseMatrix(0, 0) = e1[0];
            BaseMatrix(1, 0) = e1[1];
            BaseMatrix(2, 0) = e1[2];
            BaseMatrix(0, 2) = AxisToRotateFrom[0];
            BaseMatrix(1, 2) = AxisToRotateFrom[1];
            BaseMatrix(2, 2) = AxisToRotateFrom[2];
            BaseMatrix(0, 1) = e2[0];
            BaseMatrix(1, 1) = e2[1];
            BaseMatrix(2, 1) = e2[2];
            MBDynamicMatrix<T> RegularRotation(3);
            RegularRotation(0, 0) = std::cos(AngleToRotate);
            RegularRotation(1, 1) = std::cos(AngleToRotate);
            RegularRotation(0, 1) = -std::sin(AngleToRotate);
            RegularRotation(1, 0) = std::sin(AngleToRotate);
            //std::cout << BaseMatrix << std::endl;
            //std::cout << BaseMatrix.Transpose() << std::endl;
            //std::cout << RegularRotation << std::endl;
            MBDynamicMatrix<T> RotationMatrix = BaseMatrix * RegularRotation * BaseMatrix.Transpose();
            return(RotationMatrix);
        }
        //TODO implementera RotationFromVector
        //static MBVector3<T> RotationFromVector(MBVector3<T> CurrentVector, MBVector3<T> OrignalVector)
        //{
        //    return(MBVector3<T>(0, 0, 0));
        //}
        void Rotate(double AngleToRotate, MBDynamicVector<T> AxisToRotateFrom)
        {
            AngleToRotate *= (double)3.141592653589793238 / 180;
            AxisToRotateFrom = AxisToRotateFrom.Normalized();
            long long ColumnCount = this->VectorData.NumberOfColumns();
            MBDynamicVector3 e1;
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
            MBDynamicVector3<T> e2 = MBDynamicVector3(AxisToRotateFrom).CrossProduct(e1);
            //T Test = e1.DotProduct(AxisToRotateFrom);
            //T Test2 = e1.DotProduct(e2);
            //T Test3 = e2.DotProduct(AxisToRotateFrom);
            //nu vill vi konstruera e1 i vår nya bas, den har enbart egenskapen att dens skalar produkt med vektor ar 0.
            MBDynamicMatrix<T> BaseMatrix(3);
            BaseMatrix(0, 0) = e1[0];
            BaseMatrix(1, 0) = e1[1];
            BaseMatrix(2, 0) = e1[2];
            BaseMatrix(0, 2) = AxisToRotateFrom[0];
            BaseMatrix(1, 2) = AxisToRotateFrom[1];
            BaseMatrix(2, 2) = AxisToRotateFrom[2];
            BaseMatrix(0, 1) = e2[0];
            BaseMatrix(1, 1) = e2[1];
            BaseMatrix(2, 1) = e2[2];
            MBDynamicMatrix<T> RegularRotation(3);
            RegularRotation(0, 0) = std::cos(AngleToRotate);
            RegularRotation(1, 1) = std::cos(AngleToRotate);
            RegularRotation(0, 1) = -std::sin(AngleToRotate);
            RegularRotation(1, 0) = std::sin(AngleToRotate);
            //std::cout << BaseMatrix << std::endl;
            //std::cout << BaseMatrix.Transpose() << std::endl;
            //std::cout << RegularRotation << std::endl;
            MBDynamicMatrix<T> RotationMatrix = BaseMatrix * RegularRotation * BaseMatrix.Transpose();
            //std::cout << RotationMatrix << std::endl;
            this->VectorData = (RotationMatrix * this->VectorData.Transpose()).Transpose();
        }
        MBDynamicVector3<T> LinearInterpolation(MBDynamicVector3<T> EndVector,float NormalizedTime)
        {
            return((1 - NormalizedTime) * (*this) + NormalizedTime * EndVector);
        }
        MBDynamicVector3(T x , T y, T z)
        {
            this->VectorData = MBDynamicMatrix<T>(1, 3);
            this->VectorData(0, 0) = x;
            this->VectorData(0, 1) = y;
            this->VectorData(0, 2) = z;
        }
        MBDynamicVector3(MBDynamicMatrix<T> MatrixToConvert)
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
        MBDynamicVector3(MBDynamicVector<T> VectorToConvert) : MBDynamicVector<T>(3)
        {
            (*this)[0] = VectorToConvert[0];
            (*this)[1] = VectorToConvert[1];
            (*this)[2] = VectorToConvert[2];
        }
    };
    template<typename T>
    class MBDynamicMatrix4
    {
    private:
        MBDynamicMatrix<T> MatrixData = MBDynamicMatrix<T>(4);
        std::vector<T> ContinousData = std::vector<T>(16);
    public:
        MBMath::MBDynamicMatrix<T> GetMatrixData()
        {
            return(MatrixData);
        }
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
        friend std::ostream& operator<<(std::ostream& os, MBDynamicMatrix4<T> MatrixToPrint)
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
        friend MBDynamicMatrix4<T> operator*(const MBDynamicMatrix4<T>& Left, const MBDynamicMatrix4<T>& Right)
        {
            //nu gör vi matris multiplikation;
            long long NumberOfRows = 4;
            long long NumberOfColumns = 4;
            long long InnerIterations = 4;
            MBDynamicMatrix4<T> MatrixToReturn = MBDynamicMatrix4<T>();
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
        friend MBDynamicVector3<T> operator*(MBDynamicMatrix4<T> LeftMatrix, MBDynamicVector3<T> RightVector)
        {
            MBDynamicMatrix4 NewRightMatrix(RightVector);
            NewRightMatrix = LeftMatrix * NewRightMatrix;
            return(MBDynamicVector3<T>(NewRightMatrix[0], NewRightMatrix[1], NewRightMatrix[2]));
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
        MBDynamicMatrix4(MBDynamicVector3<T> Vector)
        {
            (*this)[0] = Vector[0];
            (*this)[1] = Vector[1];
            (*this)[2] = Vector[2];
            (*this)[3] = 1;
        }
        MBDynamicMatrix4()
        {
        }
        MBDynamicMatrix4& operator=(MBDynamicMatrix4 const&) = default;
        MBDynamicMatrix4& operator=(MBDynamicMatrix4&&) = default;
        MBDynamicMatrix4(MBDynamicMatrix<T> MatrixToCopy)
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
                //error, bli leds
                throw std::runtime_error("Invalid dimensions for initilization of DynamicMatrix");
            }
        }
    };

    
    template<typename T,size_t C>
    class MBStaticVector;
    template<typename N, size_t, size_t>
    class MBStaticMatrix;
    template<typename N, size_t I, size_t K>
    MBStaticVector<N, I> operator*(MBStaticMatrix<N, I, K> const& LeftMatrix, MBStaticVector<N, I> const& RightVector);
    template <typename T,size_t C> 
    class MBStaticVector
    {
    protected:
        T m_InternalArray[C];
        template<typename N,size_t X,size_t Y>
        friend class MBStaticMatrix;
        template<typename Z,size_t I,size_t K>
        friend MBStaticVector<Z, I> operator*(MBStaticMatrix<Z,I,K> const& LeftMatrix, MBStaticVector<Z, I> const& RightVector);
        void p_ZeroValues()
        {
            for(size_t i = 0; i < C;i++)
            {
                m_InternalArray[i] = 0;
            }    
        }
    public:
        MBStaticVector()
        {
            p_ZeroValues();
        }
        //template<typename ...I>
        //MBStaticVector(I ... args)
        //{
        //    m_InternalArray = { args... };
        //}
        T& operator[](size_t Index)
        {
            return(m_InternalArray[Index]);
        }
        T const& operator[](size_t Index) const
        {
            return(m_InternalArray[Index]);
        }
        MBStaticVector& operator=(MBStaticVector const& RightVector)
        {
            for (size_t i = 0; i < C; i++)
            {
                m_InternalArray[i] = RightVector[i];
            }
            return(*this);
        }
        MBStaticVector& operator+=(MBStaticVector const& RightVector)
        {
            (*this) = (*this) + RightVector;
            return(*this);
        }
        MBStaticVector friend operator-(MBStaticVector const& LeftVector, MBStaticVector const& RightVector)
        {
            MBStaticVector ReturnValue;
            for (size_t i = 0; i < C; i++)
            {
                ReturnValue[i] = LeftVector[i] - RightVector[i];
            }
            return(ReturnValue);
        }
        MBStaticVector& operator-=(MBStaticVector const& RightVector)
        {
            (*this) = (*this) - RightVector;
            return(*this);
        }
        MBStaticVector& operator*=(T const& Scalar)
        {
            for (size_t i = 0; i < C; i++)
            {
                m_InternalArray[i] *= Scalar;
            }
            return(*this);
        }
        MBStaticVector friend operator*(MBStaticVector LeftVector, T const& RightScalar)
        {
            LeftVector *= RightScalar;
            return(LeftVector);
        }
        MBStaticVector friend operator*(T const& LeftScalar,MBStaticVector RightVector)
        {
            RightVector *= LeftScalar;
            return(RightVector);
        }
        MBStaticVector friend operator*(MBStaticVector const& LeftVector, MBStaticVector const& RightVector)
        {
            MBStaticVector ReturnValue;
            for (size_t i = 0; i < C; i++)
            {
                ReturnValue.m_InternalArray[i] = LeftVector.m_InternalArray[i] * RightVector.m_InternalArray[i];
            }
            return(ReturnValue);
        }
        bool friend operator==(MBStaticVector const& LeftVector, MBStaticVector const& RightVector)
        {
            bool ReturnValue = true;
            for (size_t i = 0; i < C; i++)
            {
                if (LeftVector.m_InternalArray[i] != RightVector.m_InternalArray[i])
                {
                    ReturnValue = false;
                    break;
                }
            }
            return(ReturnValue);
        }
        MBStaticVector friend operator+(MBStaticVector const& LeftVector, MBStaticVector const& RightVector)
        {
            MBStaticVector ReturnValue;
            for (size_t i = 0; i < C; i++)
            {
                ReturnValue[i] = LeftVector[i] + RightVector[i];
            }
            return(ReturnValue);
        }
        friend std::ostream& operator<<(std::ostream& Out, const MBStaticVector& VectorToPrint)
        {
            //std::cout << "Kommer hit" << std::endl;
            for (size_t j = 0; j < C; j++)
            {
                Out << VectorToPrint.m_InternalArray[j] << " ";
            }
            return(Out);
        }
        MBStaticVector<T,C> Normalized() const
        {
            MBStaticVector<T,C> ReturnValue = *this;
            T Magnitude = 0;
            for (size_t i = 0; i < C; i++)
            {
                Magnitude += m_InternalArray[i] *m_InternalArray[i];
            }
            Magnitude = Sqrt(Magnitude);
            if (Magnitude == 0)
            {
                return(ReturnValue);
            }
            for (size_t i = 0; i < C; i++)
            {
                ReturnValue.m_InternalArray[i] /= Magnitude;
            }
            return(ReturnValue);
        }

        template <typename U,size_t S>
        friend U DotProduct(MBStaticVector<U,S> const& LeftVector, MBStaticVector<U,S> const& RightVector);
    };

    template <typename U,size_t S>
    U DotProduct(MBStaticVector<U,S> const& LeftVector, MBStaticVector<U,S> const& RightVector)
    {
        U ReturnValue = 0;
        for (size_t i = 0; i < S; i++)
        {
            ReturnValue += LeftVector.m_InternalArray[i] * RightVector.m_InternalArray[i];
        }
        return(ReturnValue);
    }

    template<typename T>
    class MBStaticVector3 : public MBStaticVector<T, 3>
    {
    private:

    public:
        MBStaticVector3()
        {

        }
        MBStaticVector3(MBStaticVector<T,3>&& VectorToSteal)
        {
            this->m_InternalArray[0] = std::move(VectorToSteal[0]);
            this->m_InternalArray[1] = std::move(VectorToSteal[1]);
            this->m_InternalArray[2] = std::move(VectorToSteal[2]);
        }
        T& operator[](size_t Index)
        {
            return(this->m_InternalArray[Index]);
        }
        T const& operator[](size_t Index) const
        {
            return(this->m_InternalArray[Index]);
        }
        MBStaticVector3(T X,T Y,T Z)
        {
            this->m_InternalArray[0] = std::move(X);
            this->m_InternalArray[1] = std::move(Y);
            this->m_InternalArray[2] = std::move(Z);
        }
        void Rotate(double AngleToRotate, MBStaticVector3<T> const& SuppliedAxis)
        {
            //AngleToRotate *= (double)3.141592653589793238 / 180;
            //MBStaticVector3<T> AxisToRotateFrom = SuppliedAxis.Normalized();
            //MBStaticVector3<T> e1;
            //bool e1Finished = false;
            ////varför behövs detta?
            //for (size_t i = 0; i < 3; i++)
            //{
            //    if (AxisToRotateFrom[i] == 0)
            //    {
            //        e1[i] = 1;
            //        e1Finished = true;
            //        break;
            //    }
            //}
            //if (!e1Finished)
            //{
            //    e1[0] = -AxisToRotateFrom[1];
            //    e1[1] = AxisToRotateFrom[0];
            //}
            ////
            //e1 = e1.Normalized();
            //MBStaticVector3<T> e2 = CrossProduct(AxisToRotateFrom,e1);
            //MBStaticMatrix<T,3,3> BaseMatrix;
            //BaseMatrix(0, 0) = e1[0];
            //BaseMatrix(1, 0) = e1[1];
            //BaseMatrix(2, 0) = e1[2];
            //BaseMatrix(0, 2) = AxisToRotateFrom[0];
            //BaseMatrix(1, 2) = AxisToRotateFrom[1];
            //BaseMatrix(2, 2) = AxisToRotateFrom[2];
            //BaseMatrix(0, 1) = e2[0];
            //BaseMatrix(1, 1) = e2[1];
            //BaseMatrix(2, 1) = e2[2];
            //MBStaticMatrix<T,3,3> RegularRotation;
            //RegularRotation(0, 0) = std::cos(AngleToRotate);
            //RegularRotation(1, 1) = std::cos(AngleToRotate);
            //RegularRotation(0, 1) = -std::sin(AngleToRotate);
            //RegularRotation(1, 0) = std::sin(AngleToRotate);
            //RegularRotation(2, 2) = 1;
            MBStaticMatrix<T, 3, 3> RotationMatrix = GetRotationMatrix(AngleToRotate, SuppliedAxis);
            *this = RotationMatrix * (*this);
        }

        template <typename U>
        friend MBStaticVector3<U> LinearInterpolation(MBStaticVector3<U> const& StartVector, MBStaticVector3<U> const& EndVector, double NormalizedTime);
        template <typename U>
        friend MBStaticVector3<U> CrossProduct(MBStaticVector3<U> const& LeftVector, MBStaticVector3<U> const& RightVector);


    };
    template<typename T>
    MBStaticVector3<T> CrossProduct(MBStaticVector3<T> const& LeftVector, MBStaticVector3<T> const& RightVector)
    {
        MBStaticVector3<T> ReturnValue;
        ReturnValue.m_InternalArray[0] = LeftVector[1] * RightVector[2] - LeftVector[2] * RightVector[1];
        ReturnValue.m_InternalArray[1] = LeftVector[2] * RightVector[0] - LeftVector[0] * RightVector[2];
        ReturnValue.m_InternalArray[2] = LeftVector[0] * RightVector[1] - LeftVector[1] * RightVector[0];
        return(ReturnValue);
    }
    template <typename U>
    MBStaticVector3<U> LinearInterpolation(MBStaticVector3<U> const& StartVector, MBStaticVector3<U> const& EndVector, double NormalizedTime)
    {
        return(((1 - NormalizedTime) * StartVector) + (NormalizedTime * EndVector));
    }
    constexpr bool MBSTATIC_MATRIX_ERRORCHECK = true;
    template <typename T, size_t I, size_t K>
    class MBStaticMatrix
    {
    protected:
        T m_InternalArray[I*K];
        T static p_GetDotProduct(const T* LeftArray, size_t LeftArrayElementOffset, const T* RightArray, size_t RightArrayOffset, size_t NumberOfElements)
        {
            T ReturnValue = 0;
            for (size_t i = 0; i < NumberOfElements; i++)
            {
                ReturnValue += (LeftArray[i * LeftArrayElementOffset]) * (RightArray[i * RightArrayOffset]);
            }
            return(ReturnValue);
        }

        //Kanske den absolut mest fula hacket jag gjort på länge, men att frienda till template friends på det här sättet var jobbigt af, så ger den här klassen helt enkelt möjligheten
        //att extrahera pointern till friend operators som behöver den
        template<size_t C>
        static T* p_GetArrayPointer(MBStaticVector<T, C>& Vector)
        {
            return(Vector.m_InternalArray);
        }
        template<size_t C>
        static const T* p_GetArrayPointer(MBStaticVector<T, C> const& Vector)
        {
            return(Vector.m_InternalArray);
        }
        void p_ZeroArray()
        {
            for (size_t i = 0; i < I*K; i++)
            {
                m_InternalArray[i] = 0;
            }
        }
        void p_Swap(T& LeftValue, T& RightValue)
        {
            T Temp;
            Temp = std::move(LeftValue);
            LeftValue = std::move(RightValue);
            RightValue = std::move(LeftValue);
        }
        void p_SwapRow(size_t FirstRowIndex, size_t SecondRowIndex)
        {
            for (size_t i = 0; i < K; i++)
            {
                p_Swap((*this)(FirstRowIndex, i), (*this)(SecondRowIndex, i));
            }
        }
        void p_SwapColumn(size_t FirstColumnIndex, size_t SecondColumnIndex)
        {
            for (size_t i = 0; i < I; i++)
            {
                p_Swap((*this)(i, FirstColumnIndex), (*this)(i, SecondColumnIndex));
            }
        }
        void p_SubtractRow(size_t ValueRowIndex, size_t RowToChangeIndex, T const& Multiple)
        {
            for (size_t i = 0; i < K; i++)
            {
                (*this)(RowToChangeIndex, i) -= (*this)(ValueRowIndex, i) * Multiple;
            }
        }
        void p_MultiplyRow(size_t RowToMultiply, T const& Multiple)
        {
            for (size_t i = 0; i < K; i++)
            {
                (*this)(RowToMultiply, i) *= Multiple;
            }
        }
        //destruktiv funktion som tar datan och gauss elimineraar den, returnerar antalet switchers av rader
        void p_RowPreservering_GaussEliminate(MBStaticMatrix* InvertedMatrixToChange,size_t* PositionsToUpdate)
        {
            for (size_t CurrentRowIndex = 0; CurrentRowIndex < I; CurrentRowIndex++)
            {
                size_t FirstZeroColumn = 0;
                for (size_t i = 0; i < K; i++)
                {
                    if ((*this)(CurrentRowIndex, i) != 0)
                    {
                        FirstZeroColumn = i;
                        break;
                    }
                }
                PositionsToUpdate[CurrentRowIndex] = FirstZeroColumn;
                for (size_t i = CurrentRowIndex+1; i < I; i++)
                {
                    T& CurrentRowValue = (*this)(i, FirstZeroColumn);
                    T& PivotValue = (*this)(CurrentRowIndex, FirstZeroColumn);
                    if (CurrentRowValue != 0)
                    {
                        T SubtractMultiple = CurrentRowValue/PivotValue;
                        p_SubtractRow(CurrentRowIndex, i, SubtractMultiple);
                        if (InvertedMatrixToChange != nullptr)
                        {
                            InvertedMatrixToChange->p_SubtractRow(CurrentRowIndex,i,SubtractMultiple);
                        }
                    }
                }
            }
        }

        //returnerar antalet swaps
        size_t p_Make_GaussEliminatedMatrix_RowEchelonForm(size_t* PivotPositions,bool SwapRows,MBStaticMatrix* InvertedMatrixToChange)
        {
            size_t ReturnValue = 0;
            //vi kör på en bubble sort för den kan jag utantill och gör så vi kan räkna antal byten
            for (int i = I-1; i >= 1; i--)
            {
                for (size_t j = 0; j < i; j++)
                {
                    if (PivotPositions[j] > PivotPositions[j+1])
                    {
                        if (SwapRows)
                        {
                            p_SwapRow(PivotPositions[j], PivotPositions[j + 1]);
                            if (InvertedMatrixToChange != nullptr)
                            {
                                p_SwapRow(PivotPositions[j], PivotPositions[j + 1]);
                            }
                        }
                        std::swap(PivotPositions[j], PivotPositions[j + 1]);
                        ReturnValue += 1;
                    }
                }
            }
            return(ReturnValue);
        }
        //finnse nbart för determinant så tar in en referns för att göra det tydligt
        void p_Make_RowEchelonForm_UnitMatrix(MBStaticMatrix& InvertedMatrixToChange,size_t* PivotPositions)
        {
            for (size_t RowIndex = 1; RowIndex < I; RowIndex++)
            {
                for (int j = RowIndex -1 ; j >= 0; j--)
                {
                    T& UpperRowValue = (*this)(j, PivotPositions[RowIndex]);
                    T& PivotValue = (*this)(RowIndex, PivotPositions[RowIndex]);
                    if (UpperRowValue == 0)
                    {
                        continue;
                    }
                    T SubtractMultiple = UpperRowValue/PivotValue;
                    p_SubtractRow(RowIndex, j, SubtractMultiple);
                    InvertedMatrixToChange.p_SubtractRow(RowIndex, j, SubtractMultiple);
                }
            }
        }
    public:
        MBStaticMatrix()
        {
            p_ZeroArray();
            for (size_t i = 0; i < I*K; i++)
            {
                m_InternalArray[i] = 0;
            }
        }
        T& operator()(size_t Row, size_t Column)
        {
            size_t Index = (Row * K) + Column;
            if constexpr(MBSTATIC_MATRIX_ERRORCHECK)
            {
                if (Row >= I || Column >= K || Index >= (I * K))
                {
                    throw std::domain_error("Matrix index out of bounds");
                }
            }
            return(m_InternalArray[Index]);
        }
        T const& operator()(size_t Row, size_t Column) const
        {
            size_t Index = (Row * K) + Column;
            if constexpr (MBSTATIC_MATRIX_ERRORCHECK)
            {
                if (Row >= I || Column > K || Index >= (I * K))
                {
                    throw std::domain_error("Matrix index out of bounds");
                }
            }
            return(m_InternalArray[Index]);
        }
        MBStaticMatrix<T, I, K> friend operator+(MBStaticMatrix<T, I, K> const& LeftMatrix, MBStaticMatrix<T, I, K> const& RightMatrix)
        {
            MBStaticMatrix<T, I, K> ReturnValue;
            for (size_t i = 0; i < I; i++)
            {
                for (size_t k = 0; k < K; k++)
                {
                    ReturnValue(i, k) = LeftMatrix(i, k) + RightMatrix(i, k);
                }
            }
            return(ReturnValue);
        }
        //TODO kanske kan optimeras med använding av copy-elasion medveten kod
        MBStaticMatrix<T, I, K> friend operator-(MBStaticMatrix<T, I, K> const& LeftMatrix, MBStaticMatrix<T, I, K> const& RightMatrix)
        {
            MBStaticMatrix<T, I, K> ReturnValue;
            for (size_t i = 0; i < I; i++)
            {
                for (size_t k = 0; k < K; k++)
                {
                    ReturnValue(i, k) = LeftMatrix(i, k) - RightMatrix(i, k);
                }
            }
            return(ReturnValue);
        }
        template<size_t P>
        MBStaticMatrix<T, I, P> friend operator*(MBStaticMatrix<T, I, K> const& LeftMatrix, MBStaticMatrix<T, I, P> const& RightMatrix)
        {
            MBStaticMatrix<T, I, P> ReturnValue;
            for (size_t i = 0; i < I; i++)
            {
                for (size_t j = 0; j < P; j++)
                {
                    ReturnValue(i,j) = p_GetDotProduct(&LeftMatrix.m_InternalArray[i*K], 1, &RightMatrix.m_InternalArray[j], P, P);
                }
            }
            return(ReturnValue);
        }
        MBStaticMatrix& operator*=(T const& Scalar)
        {
            for (size_t i = 0; i < I * K;i++)
            {
                m_InternalArray[i] *= Scalar;
            }
            return(*this);
        }

        friend MBStaticMatrix operator*(MBStaticMatrix LeftMatrix, T const& RightScalar)
        {
            LeftMatrix *= RightScalar;
            return(LeftMatrix);
        }
        friend MBStaticMatrix operator*(T const& LeftScalar, MBStaticMatrix RightMatrix)
        {
            RightMatrix *= LeftScalar;
            return(RightMatrix);
        }
        friend MBStaticVector<T, I> operator*(MBStaticMatrix const& LeftMatrix, MBStaticVector<T, I> const& RightVector)
        {
            MBStaticVector<T, I> ReturnValue;
            for (size_t i = 0; i < I; i++)
            {
                ReturnValue[i] = MBStaticMatrix::p_GetDotProduct(&LeftMatrix.m_InternalArray[i * K], 1, MBStaticMatrix::p_GetArrayPointer(RightVector), 1,I);
            }
            return(ReturnValue);
        }

        friend std::ostream& operator<<(std::ostream& Out, const MBStaticMatrix& MatrixToPrint)
        {
            for (size_t i = 0; i < I; i++)
            {
                for (size_t j = 0; j < K; j++)
                {
                    Out << MatrixToPrint.m_InternalArray[i * K + j] << " ";
                }
                Out << "\n";
            }
            return(Out);
        }
        
        MBStaticMatrix<T, K, I> Transpose() const
        {
            MBStaticMatrix<T, K, I> ReturnValue;
            for (size_t i = 0; i < I; i++)
            {
                for (size_t j = 0; j < K; j++)
                {
                    ReturnValue(j, i) = m_InternalArray[i * K + j];
                }
            }
            return(ReturnValue);
        }
        //friend std::ofstream& operator<< <T, I, K>(std::ofstream& out, MBStaticMatrix const& MatrixToPrint);
        template <typename U, size_t S>
        friend MBStaticMatrix<U, S, S> GetInverse(MBStaticMatrix<U, S,S> MatrixToInvert);
    };
    template <typename U, size_t S>
    MBStaticMatrix<U, S, S> GetInverse(MBStaticMatrix<U, S,S> MatrixToOperateOn)
    {
        MBStaticMatrix<U,S,S> ReturnValue;
        for (size_t i = 0; i < S; i++)
        {
            ReturnValue(i, i) = 1;
        }
        size_t PivotPositions[S];
        MatrixToOperateOn.p_RowPreservering_GaussEliminate(&ReturnValue, PivotPositions);
        MatrixToOperateOn.p_Make_GaussEliminatedMatrix_RowEchelonForm(PivotPositions, true, &ReturnValue);
        MatrixToOperateOn.p_Make_RowEchelonForm_UnitMatrix(ReturnValue, PivotPositions);
        return(ReturnValue);
    }
    template<typename T>
    class MBStaticMatrix4 : public MBStaticMatrix<T, 4, 4>
    {

    private:

    public:
        MBStaticMatrix4()
        {
            (*this)(0, 0) = 1;
            (*this)(1, 1) = 1;
            (*this)(2, 2) = 1;
            (*this)(3, 3) = 1;
        }
        const T* GetContinousData() const
        {
            return(this->m_InternalArray);
        }
        T& operator()(size_t i, size_t j)
        {
            return(MBStaticMatrix<T, 4, 4>::operator()(i, j));
        }
        T const& operator()(size_t i, size_t j) const
        {
            return(MBStaticMatrix<T, 4, 4>::operator()(i, j));
        }
        MBStaticMatrix4(MBStaticMatrix<T, 4, 4>const& MatrixToCopy)
        {
            for (size_t i = 0; i < 4; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    (*this)(i, j) = MatrixToCopy(i, j);
                }
            }
        }
        MBStaticMatrix4(MBStaticMatrix<T, 4, 4>&& MatrixToSteal)
        {
            for (size_t i = 0; i < 4; i++)
            {
                for (size_t j = 0; j < 4; j++)
                {
                    (*this)(i, j) = std::move(MatrixToSteal(i, j));
                }
            }
        }
        MBStaticMatrix4(MBStaticVector3<T> const& InitaliserVector)
        {
            for (size_t i = 0; i < 3;i++)
            {
                (*this)(i, i) = InitaliserVector[i];
            }
            (*this)(3, 3) = 1;
        }
        MBStaticMatrix4(MBStaticMatrix<T,3,3> InitaliserVector)
        {
            for (size_t i = 0; i < 3;i++)
            {
                for (size_t j = 0; j < 3;j++)
                {
                    (*this)(i, j) = InitaliserVector(i,j);
                }
            }
            (*this)(3, 3) = 1;
        }
        T GetDeterminant() const
        {
            //Behöver kopiera matrixen för detta, aningen innefektivt men tror inte riktigt det finns något annat sätt
            MBStaticMatrix4 MatrixToOperateOn = *this;
            size_t PivotPositions[4];
            MatrixToOperateOn.p_RowPreservering_GaussEliminate(nullptr, PivotPositions);
            //std::cout << MatrixToOperateOn << std::endl;
            size_t NumberOfSwaps = MatrixToOperateOn.p_Make_GaussEliminatedMatrix_RowEchelonForm(PivotPositions, false, nullptr);
            //std::cout << MatrixToOperateOn << std::endl;
            T Determinant = 1;
            for (size_t i = 0; i < 4;i++)
            {
                Determinant *= MatrixToOperateOn(i, PivotPositions[i]);
            }
            if ((NumberOfSwaps & 1) != 0)
            {
                Determinant *= -1;
            }
            return(Determinant);
        }
        MBStaticMatrix4 GetInverse() const
        {
            MBStaticMatrix4 ReturnValue;
            MBStaticMatrix4 MatrixToOperateOn = *this;
            size_t PivotPositions[4];
            MatrixToOperateOn.p_RowPreservering_GaussEliminate(&ReturnValue, PivotPositions);
            MatrixToOperateOn.p_Make_GaussEliminatedMatrix_RowEchelonForm(PivotPositions, true, &ReturnValue);
            MatrixToOperateOn.p_Make_RowEchelonForm_UnitMatrix(ReturnValue, PivotPositions);
            return(ReturnValue);
        }
    };

    template <typename T>
    MBStaticMatrix<T,3,3> GetRotationMatrix(double AngleToRotate, MBStaticVector3<T> const& SuppliedAxis)
    {
        AngleToRotate *= (double)3.141592653589793238 / 180;
        MBStaticVector3<T> AxisToRotateFrom = SuppliedAxis.Normalized();
        MBStaticVector3<T> e1;
        bool e1Finished = false;
        //varför behövs detta?
        for (size_t i = 0; i < 3; i++)
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
        //
        e1 = e1.Normalized();
        MBStaticVector3<T> e2 = CrossProduct(AxisToRotateFrom, e1);
        MBStaticMatrix<T, 3, 3> BaseMatrix;
        BaseMatrix(0, 0) = e1[0];
        BaseMatrix(1, 0) = e1[1];
        BaseMatrix(2, 0) = e1[2];
        BaseMatrix(0, 2) = AxisToRotateFrom[0];
        BaseMatrix(1, 2) = AxisToRotateFrom[1];
        BaseMatrix(2, 2) = AxisToRotateFrom[2];
        BaseMatrix(0, 1) = e2[0];
        BaseMatrix(1, 1) = e2[1];
        BaseMatrix(2, 1) = e2[2];
        MBStaticMatrix<T, 3, 3> RegularRotation;
        RegularRotation(0, 0) = std::cos(AngleToRotate);
        RegularRotation(1, 1) = std::cos(AngleToRotate);
        RegularRotation(0, 1) = -std::sin(AngleToRotate);
        RegularRotation(1, 0) = std::sin(AngleToRotate);
        RegularRotation(2, 2) = 1;
        MBStaticMatrix<T, 3, 3> RotationMatrix = BaseMatrix * RegularRotation * BaseMatrix.Transpose();
        return(RotationMatrix);
    }
    template<typename T>
    MBStaticMatrix<T, 3, 3> GetRotationMatrix(float XRotation,float YRotation,float ZRotation)
    {
        XRotation *= (double)3.141592653589793238 / 180;
        YRotation *= (double)3.141592653589793238 / 180;
        ZRotation *= (double)3.141592653589793238 / 180;
        const float SinX = std::sin(XRotation);
        const float CosX = std::cos(XRotation);
        const float SinY = std::sin(YRotation);
        const float CosY = std::cos(YRotation);
        const float SinZ = std::sin(ZRotation);
        const float CosZ = std::cos(ZRotation);
        MBStaticMatrix<T,3,3> ReturnValue;
        ReturnValue(0, 0) = CosY * CosZ;
        ReturnValue(0, 1) = -CosX * SinZ + SinX * SinY * CosZ;
        ReturnValue(0, 2) = SinX * SinZ + CosX * SinY * CosZ;
        ReturnValue(1, 0) = CosY * SinZ;
        ReturnValue(1, 1) = CosX * CosZ+SinX*SinY*SinZ;
        ReturnValue(1, 2) = -SinX * CosZ+CosX*SinY*SinZ;
        ReturnValue(2, 0) = -SinY;
        ReturnValue(2, 1) = SinX*CosY;
        ReturnValue(2, 2) = CosX*CosY;
        return(ReturnValue);
    }
    template <typename T>
    class Quaternion
    {
    private:
    public:
        T a = 0;
        T i = 0;
        T j = 0;
        T k = 0;
        friend std::ostream& operator<<(std::ostream& os, Quaternion<T> QuaternionToPrint)
        {
            os << QuaternionToPrint.a << " " << QuaternionToPrint.i << " " << QuaternionToPrint.j << " " << QuaternionToPrint.k;
            return(os);
        }
        Quaternion<T>& operator+=(const Quaternion<T>& RightQuaternion)
        {
            this->a += RightQuaternion.a;
            this->i += RightQuaternion.i;
            this->j += RightQuaternion.j;
            this->k += RightQuaternion.k;
            return(*this);
        }
        Quaternion<T>& operator-=(const Quaternion<T>& RightQuaternion)
        {
            this->a -= RightQuaternion.a;
            this->i -= RightQuaternion.i;
            this->j -= RightQuaternion.j;
            this->k -= RightQuaternion.k;
            return(*this);
        }
        Quaternion<T>& operator*=(const Quaternion<T>& RightQuaternion)
        {
            MBStaticVector3<T> ThisVector = MBStaticVector3<T>(this->i, this->j, this->k);
            MBStaticVector3<T> RightVector = MBStaticVector3<T>(RightQuaternion.i, RightQuaternion.j, RightQuaternion.k);
            MBStaticVector3<T> NewVector = this->a * RightVector + RightQuaternion.a * ThisVector + CrossProduct(ThisVector,RightVector);
            this->a = this->a * RightQuaternion.a - ThisVector.DotProduct(RightVector);
            this->i = NewVector[0];
            this->j = NewVector[1];
            this->k = NewVector[2];
            return(*this);
        }
        Quaternion<T>& operator*=(const T& RightScalar)
        {
            this->a *= RightScalar;
            this->i *= RightScalar;
            this->j *= RightScalar;
            this->k *= RightScalar;
            return(*this);
        }
        Quaternion<T>& operator/=(const T& RightScalar)
        {
            this->a /= RightScalar;
            this->i /= RightScalar;
            this->j /= RightScalar;
            this->k /= RightScalar;
            return(*this);
        }
        friend Quaternion<T> operator-(const Quaternion<T>& LeftQuaternion, const Quaternion<T>& RightQuaternion)
        {
            Quaternion<T> Result(LeftQuaternion);
            Result -= RightQuaternion;
            return(Result);
        }
        friend Quaternion<T> operator+(const Quaternion<T>& LeftQuaternion, const Quaternion<T>& RightQuaternion)
        {
            Quaternion<T> Result(LeftQuaternion);
            Result += RightQuaternion;
            return(Result);
        }
        friend Quaternion<T> operator*(const Quaternion<T>& LeftQuaternion, const T& RightScalar)
        {
            Quaternion<T> Result(LeftQuaternion);
            Result *= RightScalar;
            return(Result);
        }
        friend Quaternion<T> operator*(const T& LeftScalar, const Quaternion<T>& RightQuaternion)
        {
            Quaternion<T> Result(RightQuaternion);
            Result *= LeftScalar;
            return(Result);
        }
        friend Quaternion<T> operator/(const Quaternion<T>& LeftQuaternion, const T &RightScalar)
        {
            Quaternion<T> Result(LeftQuaternion);
            Result /= RightScalar;
            return(Result);
        }
        friend Quaternion<T> operator*(const Quaternion<T>& LeftQuaternion, const Quaternion<T>& RightQuaternion)
        {
            Quaternion<T> Result(LeftQuaternion);
            Result *= RightQuaternion;
            return(Result);
        }
        Quaternion(T NewA, T NewI, T NewJ, T NewK)
        {
            this->a = NewA;
            this->i = NewI;
            this->j = NewJ;
            this->k = NewK;
        }
        Quaternion(float RotationAngle, MBStaticVector3<T> const& RotationVector)
        {
            //omvandlar rotationen från euler till radianer
            RotationAngle = RotationAngle * 3.1415926535 / 180;
            RotationVector = RotationVector.Normalized();
            RotationVector = RotationVector * std::sin(RotationAngle / 2);
            this->a = std::cos(RotationAngle / 2);
            this->i = RotationVector[0];
            this->j = RotationVector[1];
            this->k = RotationVector[2];
        }
        Quaternion()
        {

        }
        MBDynamicVector3<T> RotateVector(MBStaticVector3<T> const&  VectorToRotate)
        {
            Quaternion<T> NormalizedQuaternion = (*this);
            NormalizedQuaternion.Normalize();
            Quaternion<T> InvertedQuaternion = NormalizedQuaternion;
            InvertedQuaternion.Invert();
            Quaternion<T> VectorQuaternion = Quaternion<T>(0, VectorToRotate[0], VectorToRotate[1], VectorToRotate[2]);
            VectorQuaternion = NormalizedQuaternion * VectorQuaternion * InvertedQuaternion;
            return(MBDynamicVector3<T>(VectorQuaternion.i, VectorQuaternion.j, VectorQuaternion.k));
        }
        MBStaticMatrix<T,3,3> GetRotationMatrix()
        {
            MBStaticMatrix<T, 3, 3> ReturnValue;
            //eventeullt normaliserar vi inna, har ingen sån, men antar i alla fall att 
            ReturnValue(0, 0) = 1 - 2 * (j * j + k * k);
            ReturnValue(0, 1) = 2 * (i * j - k * a);
            ReturnValue(0, 2) = 2 * (i * k + j * a);
            ReturnValue(1, 0) = 2 * (i * j + k * a);
            ReturnValue(1, 1) = 1 - 2 * (i * i + k * k);
            ReturnValue(1, 2) = 2 * (j * k - i * a);
            ReturnValue(2, 0) = 2 * (i * k - j * a);
            ReturnValue(2, 1) = 2 * (j * k + i * a);
            ReturnValue(2, 2) = 1 - 2 * (i * i + j * j);
            return(ReturnValue);
        }
        /* snodd wikipedia kod
        Quaternion slerp(Quaternion v0, Quaternion v1, double t) {
    // Only unit quaternions are valid rotations.
    // Normalize to avoid undefined behavior.
    v0.normalize();
    v1.normalize();

    // Compute the cosine of the angle between the two vectors.
    double dot = dot_product(v0, v1);

    // If the dot product is negative, slerp won't take
    // the shorter path. Note that v1 and -v1 are equivalent when
    // the negation is applied to all four components. Fix by
    // reversing one quaternion.
    if (dot < 0.0f) {
        v1 = -v1;
        dot = -dot;
    }

    const double DOT_THRESHOLD = 0.9995;
    if (dot > DOT_THRESHOLD) {
        // If the inputs are too close for comfort, linearly interpolate
        // and normalize the result.

        Quaternion result = v0 + t*(v1 - v0);
        result.normalize();
        return result;
    }

    // Since dot is in range [0, DOT_THRESHOLD], acos is safe
    double theta_0 = acos(dot);        // theta_0 = angle between input vectors
    double theta = theta_0*t;          // theta = angle between v0 and result
    double sin_theta = sin(theta);     // compute this value only once
    double sin_theta_0 = sin(theta_0); // compute this value only once

    double s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
    double s1 = sin_theta / sin_theta_0;

    return (s0 * v0) + (s1 * v1);
}
        */

        Quaternion<T> Slerp(Quaternion<T> EndQuaternion, float NormalizedTime)
        {
            //förutsätter att quaternionerna är normaliserade
            EndQuaternion.Normalize();
            //Quaternion<T> Copy = *this;
            //Copy.Normalize();
            MBStaticVector<T,4> v0;
            v0[0] = a;
            v0[1] = i;
            v0[2] = j;
            v0[3] = k;
            v0 = v0.Normalized();
            MBStaticVector<T,4> v1;
            v1[0] = EndQuaternion.a;
            v1[1] = EndQuaternion.i;
            v1[2] = EndQuaternion.j;
            v1[3] = EndQuaternion.k;

            // Compute the cosine of the angle between the two vectors.
            double dot = DotProduct( v0,v1);

            // If the dot product is negative, slerp won't take
            // the shorter path. Note that v1 and -v1 are equivalent when
            // the negation is applied to all four components. Fix by 
            // reversing one quaternion.
            if (dot < 0.0f) {
                v1 = -1 * v1;
                dot = -dot;
            }

            const double DOT_THRESHOLD = 0.9995;
            if (dot > DOT_THRESHOLD) {
                // If the inputs are too close for comfort, linearly interpolate
                // and normalize the result.

                MBStaticVector<T,4> result = v0 + (NormalizedTime * (v1 - v0));
                result = result.Normalized();
                return(Quaternion(result[0], result[1], result[2], result[3]));
            }

            // Since dot is in range [0, DOT_THRESHOLD], acos is safe
            double theta_0 = std::acos(dot);        // theta_0 = angle between input vectors
            double theta = theta_0 * NormalizedTime;          // theta = angle between v0 and result
            double sin_theta = std::sin(theta);     // compute this value only once
            double sin_theta_0 = std::sin(theta_0); // compute this value only once

            double s0 = std::cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
            double s1 = sin_theta / sin_theta_0;
            MBStaticVector<T,4> ResultVector = (s0 * v0) + (s1 * v1);
            return(Quaternion<T>(std::move(ResultVector[0]), std::move(ResultVector[1]), std::move(ResultVector[2]), std::move(ResultVector[3])));
        }
        void Normalize()
        {
            T Magnitude = Sqrt<T>(a * a + i * i + j * j + k * k);
            this->a /= Magnitude;
            this->i /= Magnitude;
            this->j /= Magnitude;
            this->k /= Magnitude;
        }
        void Invert()
        {
            T Denominator = a * a + i * i + j * j + k * k;
            this->a = a / Denominator;
            this->i = -i / Denominator;
            this->j = -j / Denominator;
            this->k = -k / Denominator;
        }
    };
    //template<typename T, size_t I, size_t K>
    //std::ofstream& operator<<(std::ostream& os, const MBStaticMatrix<T, I, K>& L)
    //{
    //    for (size_t i = 0; i < I; i++)
    //    {
    //        for (size_t j = 0; j < K; j++)
    //        {
    //            out << MatrixToPrint.m_InternalArray[i * K + j] << " ";
    //        }
    //        out << "\n";
    //    }
    //    return(out);
    //}

};


//MBMath::MBMatrix<float> Test();
//början på cpp filen
namespace MBMath
{
    template<typename T> MBDynamicMatrix<T> operator+(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right)
    {
        MBDynamicMatrix<T> MatrixToReturn(Left.NumberOfRows(),Left.NumberOfColumns());
        for (size_t i = 0; i < Left.MatrixData.size(); i++)
        {
            MatrixToReturn.MatrixData[i] = Left.MatrixData[i] + Right.MatrixData[i];
        }
    }
    template<typename T> MBDynamicMatrix<T> operator-(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right)
    {
        MBDynamicMatrix<T> MatrixToReturn(Left.NumberOfRows(),Left.NumberOfColumns());
        for (size_t i = 0; i < Left.MatrixData.size(); i++)
        {
            MatrixToReturn.MatrixData[i] = Left.MatrixData[i] - Right.MatrixData[i];
        }
    }
    template<typename T> MBDynamicMatrix<T> operator*(const MBDynamicMatrix<T>& Left, const MBDynamicMatrix<T>& Right)
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
        MBDynamicMatrix<T> MatrixToReturn(NumberOfRows, NumberOfColumns);
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
    MBDynamicMatrix<T> operator*(const MBDynamicMatrix<T>& Left, long long Right)
    {
        long long NumberOfRows = Left.NumberOfRows();
        long long NumberOfColumns = Left.NumberOfColumns();
        MBDynamicMatrix<T> MatrixToReturn(NumberOfRows,NumberOfColumns);
        for (size_t i = 0; i < NumberOfRows; i++)
        {
            for (size_t j = 0; j < NumberOfColumns; j++)
            {
                MatrixToReturn(i, j) = Left(i,j)*Right;
            }
        }
        return(MatrixToReturn);
    }
    template<typename T> MBDynamicMatrix<T> operator*(long long Left, const MBDynamicMatrix<T>& Right)
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
    template<typename T> MBDynamicMatrix<T> operator*(double Left, const MBDynamicMatrix<T>& Right)
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
    template<typename T> MBDynamicMatrix<T> operator*(const MBDynamicMatrix<T> Left, double Right)
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
    template <typename T>
    using MBMatrix4 = MBStaticMatrix4<T>;
    
    template <typename T>
    using MBVector3 = MBStaticVector3<T>;
}
//#include<MBMatrix.cpp>
