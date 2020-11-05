#pragma once
#include <vector>
int Factorial(int n)
{
	int Resultat = 1;
	for (int i = 2; i <= n; i++)
	{
		Resultat *= i;
	}
	return(Resultat);
}


template<class DataTyp>
class Permutations
{
private:
	std::vector<DataTyp> Source;
	void PopulatePermutationerna(int CurrentDepth, std::vector<int> AvailablePositions, std::vector<int>* Result, int* ListOffset);
public:
	std::vector<std::vector<int>> Permutationerna;
	Permutations(std::vector<DataTyp>);
	~Permutations();
	std::vector<DataTyp> operator[](int Index)
	{
		std::vector<DataTyp> Returv�rde = std::vector<DataTyp>(Source.size());
		for (int i = 0; i < Returv�rde.size();i++)
		{
			Returv�rde[i] = Source[Permutationerna[Index][i]];
		}
		return(Returv�rde);
	}
};

template<class DataTyp> Permutations<DataTyp>::Permutations(std::vector<DataTyp> Datan)
{
	Source = Datan;
	//nu vill vi faktiskt ta och g�ra permutationerna n�r vi v�l har datan
	//vi tar ej h�nsyn till faktiska v�rdena av sakerna, enbart positionen
	//vi kan ju veta antalet permutationer genom att anv�nda permutationer av l�ngden av datan, dvs fakultet
	Permutationerna = std::vector<std::vector<int>>(Factorial(Datan.size()), std::vector<int>(Datan.size(), 0));
	//nu �r det bara att faktiskt l�gga till det som ska vara i f�r fina permutationer
	std::vector<int> AvaialbePositionsBeginning = std::vector<int>(Source.size(), 0);
	for (int i = 0; i < AvaialbePositionsBeginning.size();i++)
	{
		AvaialbePositionsBeginning[i] = i;
	}
	PopulatePermutationerna(0,AvaialbePositionsBeginning,nullptr,nullptr);
}
template<class DataTyp> void Permutations<DataTyp>::PopulatePermutationerna(int CurrentDepth, std::vector<int> AvailablePositions,std::vector<int>* Result,int* ListOffset)
{
	if(CurrentDepth == 0)
	{
		//f�rsta iterationen, vi skapar en ny vector d�r vi sparar v�rdet, sedan tar vi bort det
		Result = new std::vector<int>(Source.size(), 0);
		ListOffset = new int;
		*ListOffset = 0;
	}
	for (int i = 0; i < AvailablePositions.size(); i++)
	{
		//vi tar en available position som v�r
		(*Result)[CurrentDepth] = AvailablePositions[i];
		//det �r nu vi callar den rekursiva delen
		//f�rst skapar vi en ny vector som �r lika med v�r available, och tar bort den vi tog, och passar den vidare
		std::vector<int> PositionsAvailableNext = AvailablePositions;
		PositionsAvailableNext.erase(PositionsAvailableNext.begin() + i);
		PopulatePermutationerna(CurrentDepth + 1, PositionsAvailableNext, Result,ListOffset);
	}
	if (CurrentDepth == Source.size())
	{
		//den sista delen i f�r vina rekursiva grej sak, vi tar nu och assignar v�rdet som det ska vara
		Permutationerna[*ListOffset] = *Result;
		*ListOffset += 1;
	}
	if (CurrentDepth == 0)
	{
		//allt har h�nt, alla iterations, vi kan nu safely deleta v�r pointer
		delete Result;
	}
}

template<class DataTyp> Permutations<DataTyp>::~Permutations()
{

}