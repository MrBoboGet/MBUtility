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
	//vi anv�nder regular continued fraction s� det st�mmer med pells lemma
	//std::unordered_map<Vector2D, int> TidigareV�rden = std::unordered_map<Vector2D, int>(0);
	//a i [a|x1,x2,x3..] = (int)floor(sqrt(n))
	//vi vet att br�ket alltid kommer vara p� formen (sqrt(n) + x)/y, vi representerar detta i en vector2d
	Vector2D Br�ket = Vector2D(0, 1); //steg 1, sqrt(n) enbart
	//f�rsta delen av resultatet �r ju heltalsdelen a
	Result.push_back(a);
	//nu bev�ver bi ber�kna x1, dvs vilket tal som sedan ska anv�ndas f�r n�sta del av algoritmen
	//(sqrt(14)+t)/n = c + 1/x1 -> x1 =	n/(sqrt(14)+t-cn) ->  (n*(sqrt(14)-(t-cn))) /(14 - (t-cn)^2) -> sqrt(inputv�rdet)-(t-cn)/((inputv�rdet-(t-cn)^2)/n)
	int UrsprungligT�ljare = Br�ket.x;
	Br�ket.x = -(Br�ket.x - a * Br�ket.y);
	Br�ket.y = ((n - (UrsprungligT�ljare - Br�ket.y * a) * (UrsprungligT�ljare - Br�ket.y * a)) / Br�ket.y);
	//TidigareV�rden[Br�ket] = 1;
	int Depth = 1;
	int An = 0;
	while (true)
	{
		An = (int)((Br�ket.x + sqrt(n)) / (float)Br�ket.y);
		int UrsprungligT�ljare = Br�ket.x;
		Br�ket.x = -(Br�ket.x - An * Br�ket.y);
		Br�ket.y = ((n - (UrsprungligT�ljare - Br�ket.y * An) * (UrsprungligT�ljare - Br�ket.y * An)) / Br�ket.y);
		Result.push_back(An);
		//om detta br�ket �r det f�rsta integer v�rdet + sqrt(inputv�rdet)  ska vi stoppa, dfvsd
		if (Br�ket.x == a && Br�ket.y == 1)
		{
			//iomed detta vet vi att den kommer vara det urpsrungliga v�rdet *2 
			Result.push_back(2 * a);
			break;
		}
	}
	return(Result);
}

Vector2D ContinuedFactorEvaluator(std::vector<int> Fractionen, int AntaletIterations)
{
	Vector2D Br�ket = Vector2D(1, Fractionen[1 + (AntaletIterations - 1) % (Fractionen.size() - 1)]);
	long double KonstantAttAddera = Fractionen[0];
	long double NextKonstantIndex = (AntaletIterations - 1) % (Fractionen.size() - 1);
	for (int i = 1; i < AntaletIterations; i++)
	{
		//vi adderar br�ket med denna konstant
		long double NextConstant = Fractionen[NextKonstantIndex];
		long double UrpsrungligT�ljare = Br�ket.x;
		Br�ket.x = Br�ket.y;
		Br�ket.y = UrpsrungligT�ljare + NextConstant * Br�ket.y;
		//nu �r br�ket f�rdig�ndrat, nu tar vi fram n�sta konstant att addera
		NextKonstantIndex -= 1;
		if (NextKonstantIndex < 1)
		{
			NextKonstantIndex = Fractionen.size() - 1;
		}
	}
	//sista, vi adderar med f�sta konstanten
	long double UrpsrungligT�ljare = Br�ket.x;
	Br�ket.x += KonstantAttAddera * Br�ket.y;
	return(Br�ket);
}
//nu g�r vi s� att den returnerar en bigint ist�llet s� vi kan f� lite b�ttre stuff
const int EvaluatorIntSize = CFINTSIZE;
std::vector<ttmath::Int<CFINTSIZE>> ContinuedFactorEvaluatorBigInt(std::vector<int> Fractionen, int AntaletIterations)
{
	//Vector2D Br�ket = Vector2D(1, Fractionen[1 + (AntaletIterations - 1) % (Fractionen.size() - 1)]);
	std::vector<ttmath::Int<CFINTSIZE>> Br�ket = std::vector<ttmath::Int<CFINTSIZE>>(2);
	Br�ket[0] = 1;
	Br�ket[1] = Fractionen[1 + (AntaletIterations - 1) % (Fractionen.size() - 1)];
	ttmath::Int<CFINTSIZE> KonstantAttAddera = Fractionen[0];
	long double NextKonstantIndex = (AntaletIterations - 1) % (Fractionen.size() - 1);
	for (int i = 1; i < AntaletIterations; i++)
	{
		//vi adderar br�ket med denna konstant
		ttmath::Int<CFINTSIZE> NextConstant = Fractionen[NextKonstantIndex];
		ttmath::Int<CFINTSIZE> UrpsrungligT�ljare = Br�ket[0];
		Br�ket[0] = Br�ket[1];
		Br�ket[1] = UrpsrungligT�ljare + NextConstant * Br�ket[1];
		//nu �r br�ket f�rdig�ndrat, nu tar vi fram n�sta konstant att addera
		NextKonstantIndex -= 1;
		if (NextKonstantIndex < 1)
		{
			NextKonstantIndex = Fractionen.size() - 1;
		}
	}
	//sista, vi adderar med f�sta konstanten
	ttmath::Int<CFINTSIZE> UrpsrungligT�ljare = Br�ket[0];
	Br�ket[0] += KonstantAttAddera * Br�ket[1];
	return(Br�ket);
}
extern const int EvaluatorIntSize;