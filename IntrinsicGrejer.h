#pragma once
#include<immintrin.h>
#include <math.h>
template <class Type> class FastArray
{
private:
	Type* Values = nullptr;
	int ValueSize = 0;
public:
	FastArray(int Size, Type InitialValue = 0)
	{
		//dynamiskt allokerad array
		Values = new Type[Size];
		ValueSize = Size;
		//sätter varje värde till något som i initial value, men egentligen är det fett långsamt, kanske finns ett bätre sätt
		for (size_t i = 0; i < Size; i++)
		{
			Values[i] = InitialValue;
		}
	}
	~FastArray()
	{
		delete[] Values;
	}
	Type& operator[](int Index)
	{
		return(Values[Index]);
	}
	Type Sum()
	{
		//defierar dem registrens vi behöver, och fyller dem som növdändigt
		/*
		if (sizeof(Type) == 8)
		{
			__m256d _Accumulator, _TempValueOfArray;
			_Accumulator = _mm256_set1_pd(0);
			for (size_t i = 0; i < ValueSize/4; i++)
			{
				_TempValueOfArray = _mm256_set_pd(Values[i*4], Values[i*4 + 1], Values[i*4 + 2], Values[i*4 + 3]);
				_Accumulator = _mm256_add_pd(_Accumulator, _TempValueOfArray);
			}
			//extraherar datan och gör något litet annorlunda om vi märker att det fanns 2 saker till vi behövde addera som inte gick jämnt ut
			Type Sum = (Type)_Accumulator.m256d_f64[3] + _Accumulator.m256d_f64[2] + _Accumulator.m256d_f64[1] + _Accumulator.m256d_f64[0];
			if (ValueSize%4 != 0)
			{
				for (int i = ValueSize-4*(ValueSize/4); i < ValueSize; i++)
				{
					Sum += Values[i];
				}
			}
			return(Sum);
		}
		*/
		/*
		if (sizeof(Type) == 4)
		{

		}
		if (sizeof(Type) == 2)
		{

		}
		if (sizeof(Type) == 1)
		{

		}
		*/
	}
};
template <> double FastArray<double>::Sum()
{
	__m256d _Accumulator, _TempValueOfArray;
	_Accumulator = _mm256_set1_pd(0);
	for (size_t i = 0; i < ValueSize / 4; i++)
	{
		_TempValueOfArray = _mm256_set_pd(Values[i * 4], Values[i * 4 + 1], Values[i * 4 + 2], Values[i * 4 + 3]);
		_Accumulator = _mm256_add_pd(_Accumulator, _TempValueOfArray);
	}
	//extraherar datan och gör något litet annorlunda om vi märker att det fanns 2 saker till vi behövde addera som inte gick jämnt ut
	double Sum = (double)_Accumulator.m256d_f64[3] + _Accumulator.m256d_f64[2] + _Accumulator.m256d_f64[1] + _Accumulator.m256d_f64[0];
	if (ValueSize % 4 != 0)
	{
		for (int i = ValueSize - 4 * (ValueSize / 4); i < ValueSize; i++)
		{
			Sum += Values[i];
		}
	}
	return(Sum);
}
template <> int FastArray<int>::Sum()
{
	__m256i _Accumulator, _TempValueOfArray;
	int ValuesThatFit = 256 / (sizeof(int) * 8);

	_Accumulator = _mm256_set1_epi32(0);
	for (size_t i = 0; i < ValueSize / ValuesThatFit; i++)
	{
		int Offset = i * ValuesThatFit;
		_TempValueOfArray = _mm256_set_epi32(Values[Offset+0], Values[Offset + 1], Values[Offset + 2], Values[Offset + 3], Values[Offset + 4], Values[Offset + 5], Values[Offset + 6], Values[Offset + 7]);
		_Accumulator = _mm256_add_epi32(_Accumulator, _TempValueOfArray);
	}
	//extraherar datan och gör något litet annorlunda om vi märker att det fanns 2 saker till vi behövde addera som inte gick jämnt ut
	int Sum = (int)_Accumulator.m256i_i32[7] + _Accumulator.m256i_i32[6] + _Accumulator.m256i_i32[5] + _Accumulator.m256i_i32[4] + _Accumulator.m256i_i32[3] + _Accumulator.m256i_i32[2] + _Accumulator.m256i_i32[1] + _Accumulator.m256i_i32[0];
	if (ValueSize % ValuesThatFit != 0)
	{
		for (int i = ValueSize - ValuesThatFit * (ValueSize / ValuesThatFit); i < ValueSize; i++)
		{
			Sum += Values[i];
		}
	}
	return(Sum);
}