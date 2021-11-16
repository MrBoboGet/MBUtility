#pragma once
#include <cmath>
namespace MBMath
{
	template <typename T> T Sqrt(T NumberToSquare)
	{
		T Guess = NumberToSquare;
		if (NumberToSquare > 1.4f)
		{
			Guess /= 2;
		}
		int NumberOfIterations = 5;
		//Funktionen vi vill approximera �r x^2 - NumberToSquare = 0 -> derivatan �r 2x 
		for (size_t i = 0; i < NumberOfIterations; i++)
		{
			Guess = Guess - (Guess * Guess - NumberToSquare) / (2 * Guess);
		}
		return(Guess);
		//nu blir fr�gan hur vi f�r fram den, om roten �r mindre �n sqrt 2 tar vi bara det talet, annars tar vi talet delat p� 2
	}
	template<> 
	inline double Sqrt<double>(double NumberToSquare)
	{
		return(std::sqrt(NumberToSquare));
	}
	template<> 
	inline float Sqrt<float>(float NumberToSquare)
	{
		return(std::sqrt(NumberToSquare));
	}
};