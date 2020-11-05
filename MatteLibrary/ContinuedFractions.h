#pragma once
#include <vector>
#include<Vector2D.h>
#include<ttmathint.h>
#define CFINTSIZE 256
std::vector<int> ContinuedFractionOfIntegerRoot(int n);
Vector2D ContinuedFactorEvaluator(std::vector<int> Fractionen, int AntaletIterations);
std::vector<ttmath::Int<CFINTSIZE>> ContinuedFactorEvaluatorBigInt(std::vector<int> Fractionen, int AntaletIterations);
std::vector<int> ContinuedFractionOfIntegerRoot(int n)
{
	std::vector<int> Result = std::vector<int>(0);
	int a = (int)floor(sqrt(n));
	//sqrt(n) = a +x
	//x = n-a^2/(2a + x)
	if (a * a == n)
	{
		//inga perfecta squars
		std::cout << n << std::endl;
		assert(false);
	}
	//vi använder regular continued fraction så det stämmer med pells lemma
	//std::unordered_map<Vector2D, int> TidigareVärden = std::unordered_map<Vector2D, int>(0);
	//a i [a|x1,x2,x3..] = (int)floor(sqrt(n))
	//vi vet att bråket alltid kommer vara på formen (sqrt(n) + x)/y, vi representerar detta i en vector2d
	Vector2D Bråket = Vector2D(0, 1); //steg 1, sqrt(n) enbart
	//första delen av resultatet är ju heltalsdelen a
	Result.push_back(a);
	//nu bevöver bi beräkna x1, dvs vilket tal som sedan ska användas för nästa del av algoritmen
	//(sqrt(14)+t)/n = c + 1/x1 -> x1 =	n/(sqrt(14)+t-cn) ->  (n*(sqrt(14)-(t-cn))) /(14 - (t-cn)^2) -> sqrt(inputvärdet)-(t-cn)/((inputvärdet-(t-cn)^2)/n)
	int UrsprungligTäljare = Bråket.x;
	Bråket.x = -(Bråket.x - a * Bråket.y);
	Bråket.y = ((n - (UrsprungligTäljare - Bråket.y * a) * (UrsprungligTäljare - Bråket.y * a)) / Bråket.y);
	//TidigareVärden[Bråket] = 1;
	int Depth = 1;
	int An = 0;
	while (true)
	{
		An = (int)((Bråket.x + sqrt(n)) / (float)Bråket.y);
		int UrsprungligTäljare = Bråket.x;
		Bråket.x = -(Bråket.x - An * Bråket.y);
		Bråket.y = ((n - (UrsprungligTäljare - Bråket.y * An) * (UrsprungligTäljare - Bråket.y * An)) / Bråket.y);
		Result.push_back(An);
		//om detta bråket är det första integer värdet + sqrt(inputvärdet)  ska vi stoppa, dfvsd
		if (Bråket.x == a && Bråket.y == 1)
		{
			//iomed detta vet vi att den kommer vara det urpsrungliga värdet *2 
			Result.push_back(2 * a);
			break;
		}
	}
	return(Result);
}

Vector2D ContinuedFactorEvaluator(std::vector<int> Fractionen, int AntaletIterations)
{
	Vector2D Bråket = Vector2D(1, Fractionen[1 + (AntaletIterations - 1) % (Fractionen.size() - 1)]);
	long double KonstantAttAddera = Fractionen[0];
	long double NextKonstantIndex = (AntaletIterations - 1) % (Fractionen.size() - 1);
	for (int i = 1; i < AntaletIterations; i++)
	{
		//vi adderar bråket med denna konstant
		long double NextConstant = Fractionen[NextKonstantIndex];
		long double UrpsrungligTäljare = Bråket.x;
		Bråket.x = Bråket.y;
		Bråket.y = UrpsrungligTäljare + NextConstant * Bråket.y;
		//nu är bråket färdigändrat, nu tar vi fram nästa konstant att addera
		NextKonstantIndex -= 1;
		if (NextKonstantIndex < 1)
		{
			NextKonstantIndex = Fractionen.size() - 1;
		}
	}
	//sista, vi adderar med fösta konstanten
	long double UrpsrungligTäljare = Bråket.x;
	Bråket.x += KonstantAttAddera * Bråket.y;
	return(Bråket);
}
//nu gör vi så att den returnerar en bigint istället så vi kan få lite bättre stuff
const int EvaluatorIntSize = CFINTSIZE;
std::vector<ttmath::Int<CFINTSIZE>> ContinuedFactorEvaluatorBigInt(std::vector<int> Fractionen, int AntaletIterations)
{
	//Vector2D Bråket = Vector2D(1, Fractionen[1 + (AntaletIterations - 1) % (Fractionen.size() - 1)]);
	std::vector<ttmath::Int<CFINTSIZE>> Bråket = std::vector<ttmath::Int<CFINTSIZE>>(2);
	Bråket[0] = 1;
	Bråket[1] = Fractionen[1 + (AntaletIterations - 1) % (Fractionen.size() - 1)];
	ttmath::Int<CFINTSIZE> KonstantAttAddera = Fractionen[0];
	long double NextKonstantIndex = (AntaletIterations - 1) % (Fractionen.size() - 1);
	for (int i = 1; i < AntaletIterations; i++)
	{
		//vi adderar bråket med denna konstant
		ttmath::Int<CFINTSIZE> NextConstant = Fractionen[NextKonstantIndex];
		ttmath::Int<CFINTSIZE> UrpsrungligTäljare = Bråket[0];
		Bråket[0] = Bråket[1];
		Bråket[1] = UrpsrungligTäljare + NextConstant * Bråket[1];
		//nu är bråket färdigändrat, nu tar vi fram nästa konstant att addera
		NextKonstantIndex -= 1;
		if (NextKonstantIndex < 1)
		{
			NextKonstantIndex = Fractionen.size() - 1;
		}
	}
	//sista, vi adderar med fösta konstanten
	ttmath::Int<CFINTSIZE> UrpsrungligTäljare = Bråket[0];
	Bråket[0] += KonstantAttAddera * Bråket[1];
	return(Bråket);
}
extern const int EvaluatorIntSize;