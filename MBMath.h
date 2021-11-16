#pragma once
namespace MBMath
{
	constexpr double Pi = 3.1415926535;
	constexpr double DegreeToRadian(double DegreeToConvert)
	{
		return((DegreeToConvert / 180) * Pi);
	}
	constexpr double RadianToDegree(double RadianToConvert)
	{
		return((RadianToConvert / Pi) * 180);
	}
	template <typename T> int sign(T val) {
		return (T(0) < val) - (val < T(0));
	}
}