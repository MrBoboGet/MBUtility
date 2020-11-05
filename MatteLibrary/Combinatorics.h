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
		std::vector<DataTyp> Returvärde = std::vector<DataTyp>(Source.size());
		for (int i = 0; i < Returvärde.size();i++)
		{
			Returvärde[i] = Source[Permutationerna[Index][i]];
		}
		return(Returvärde);
	}
};

template<class DataTyp> Permutations<DataTyp>::Permutations(std::vector<DataTyp> Datan)
{
	Source = Datan;
	//nu vill vi faktiskt ta och göra permutationerna när vi väl har datan
	//vi tar ej hänsyn till faktiska värdena av sakerna, enbart positionen
	//vi kan ju veta antalet permutationer genom att använda permutationer av längden av datan, dvs fakultet
	Permutationerna = std::vector<std::vector<int>>(Factorial(Datan.size()), std::vector<int>(Datan.size(), 0));
	//nu är det bara att faktiskt lägga till det som ska vara i får fina permutationer
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
		//första iterationen, vi skapar en ny vector där vi sparar värdet, sedan tar vi bort det
		Result = new std::vector<int>(Source.size(), 0);
		ListOffset = new int;
		*ListOffset = 0;
	}
	for (int i = 0; i < AvailablePositions.size(); i++)
	{
		//vi tar en available position som vår
		(*Result)[CurrentDepth] = AvailablePositions[i];
		//det är nu vi callar den rekursiva delen
		//först skapar vi en ny vector som är lika med vår available, och tar bort den vi tog, och passar den vidare
		std::vector<int> PositionsAvailableNext = AvailablePositions;
		PositionsAvailableNext.erase(PositionsAvailableNext.begin() + i);
		PopulatePermutationerna(CurrentDepth + 1, PositionsAvailableNext, Result,ListOffset);
	}
	if (CurrentDepth == Source.size())
	{
		//den sista delen i får vina rekursiva grej sak, vi tar nu och assignar värdet som det ska vara
		Permutationerna[*ListOffset] = *Result;
		*ListOffset += 1;
	}
	if (CurrentDepth == 0)
	{
		//allt har hänt, alla iterations, vi kan nu safely deleta vår pointer
		delete Result;
	}
}

template<class DataTyp> Permutations<DataTyp>::~Permutations()
{

}