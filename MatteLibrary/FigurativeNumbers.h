#pragma once
#include <math.h>
int Triangle(int n)
{
	return(n * (n + 1) / 2);
}
//vi kollar om det närmsta heltal efter rotifiering är ett triangekl tal efter vi triangulerar den
bool IsTriangle(int n)
{
	return(n == Triangle(round(PqSolver(1, -2 * (double)n))));
}
int Square(int n)
{
	return(n * n);
}
bool IsSquare(int n)
{
	return(n == Square(round(PqSolver(0, -(double)n))));
}
int Pentagonal(int n)
{
	return(n * (3 * n - 1) / 2);
}
bool IsPentagonal(int n)
{
	return(n == Pentagonal(round(PqSolver(-1 / (float)3, -(double)n * 2 / (float)3))));
}
int Hexagonal(int n)
{
	return(n * (2 * n - 1));
}
bool IsHexagonal(int n)
{
	return(n == Hexagonal(round(PqSolver(-1 / (float)2, -(double)n / 2))));
}
int Heptagonal(int n)
{
	return(n * (5 * n - 3) / 2);
}
bool IsHeptagonal(int n)
{
	return(n == Heptagonal(round(PqSolver(-3 / (float)5, -(double)n * 2 / (float)5))));
}
int Octagonal(int n)
{
	return(n * (3 * n - 2));
}
bool IsOctagonal(int n)
{
	return(n == Octagonal(round(PqSolver(-2 / (float)3, -(double)n / 3))));
}
