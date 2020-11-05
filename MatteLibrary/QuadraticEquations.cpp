#include <QuadraticEquations.h>
double PqSolver(double P, double Q)
{
	double UnderSquareRoot = (P / 2) * (P / 2) - Q;
	if (UnderSquareRoot >= 0)
	{
		return((-P / 2) + sqrt(UnderSquareRoot));
	}
	else
	{
		assert(false);
	}
}