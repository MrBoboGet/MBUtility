#pragma once
#include <vector>
#include <math.h>
#include <MatteLibrary/TalTeori.h>
std::vector<int> LinearDiofSolver(int XKoef, int YKoef, int Sum)
{
	struct EquationLine
	{
		int k;
		int r;
	};
	std::vector<int> ReturnValue = {};
	std::vector<EquationLine> Lines = {};
	if (Sum%gcd(XKoef,YKoef) != 0)
	{
		return(ReturnValue);
	}
	int GCDNumber = gcd(XKoef, YKoef);
	XKoef /= GCDNumber;
	YKoef /= GCDNumber;
	Sum /= GCDNumber;
	EquationLine FirstLine;
	int A = Max(XKoef, YKoef);
	int B;
	if (A == XKoef)
	{
		B = YKoef;
	}
	else
	{
		B = XKoef;
	}
	//FirstLine.C = B;
	FirstLine.k = A/B;
	FirstLine.r = A%B;
	Lines.push_back(FirstLine);
	EquationLine SecondLine;
	SecondLine.k = B / FirstLine.r;
	SecondLine.r = B % FirstLine.r;
	//SecondLine.C = FirstLine.r;
	Lines.push_back(SecondLine);
	//nu populatar vi vår line grej
	int K = SecondLine.k;
	int R = SecondLine.r;
	int LineNumber = 3;
	while (R != 1)
	{
		EquationLine NewLine;
		NewLine.k = Lines[LineNumber - 1 - 2].r / Lines[LineNumber - 1 - 1].r;
		NewLine.r = Lines[LineNumber - 1 - 2].r % Lines[LineNumber - 1 - 1].r;
		R = NewLine.r;
		LineNumber += 1;
		Lines.push_back(NewLine);
	}
	//nu är alla rader klara, så vi går baklänges genom 
	int N = -(Lines[Lines.size() - 1].k);
	int N_1 = 1;
	for (int i = Lines.size()-1-2; i >= 0; i--)
	{
		int z = N_1 - N*(Lines[i+1].k);
		N_1 = N;
		N = z;
		//undantagsfall för 
	}
	//nu måste vi bara se till att vi får svaret i a och b, dvs, vi måste ändra n till att beskrigva a
	int AMultipel = N;
	int BMultipel = N_1 - (A / B) * N;
	if (A == XKoef)
	{
		ReturnValue.push_back(AMultipel* Sum);
		ReturnValue.push_back(BMultipel* Sum);
	}
	else
	{
		ReturnValue.push_back(BMultipel* Sum);
		ReturnValue.push_back(AMultipel* Sum);
	}
	return(ReturnValue);
}