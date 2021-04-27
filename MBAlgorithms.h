#pragma once
namespace MBAlgorithms
{
	template<typename Container, typename Value> int BinarySearch(Container const& ContainerToSearch, Value const& ValueToGet)
	{
		int ReturnValue = -1;
		int NumberOfElements = ContainerToSearch.size();
		int LowerLimit = -1;
		int UpperLimit = NumberOfElements;
		if (NumberOfElements == 0)
		{
			return(-1);
		}
		while (true)
		{
			int CurrentGuess = (LowerLimit + UpperLimit) / 2;
			if (ContainerToSearch[CurrentGuess] < ValueToGet)
			{
				LowerLimit = CurrentGuess;
			}
			else
			{
				UpperLimit = CurrentGuess;
			}

			if (UpperLimit - LowerLimit == 1 || ContainerToSearch[CurrentGuess] == ValueToGet)
			{
				if (ContainerToSearch[CurrentGuess] == ValueToGet)
				{
					ReturnValue = CurrentGuess;
				}
				break;
			}
		}
		return(ReturnValue);
	}
	template<typename Container, typename Value,typename Compare> int BinarySearch(Container const& ContainerToSearch, Value const& ValueToGet,Compare CompareFunction)
	{
		int ReturnValue = -1;
		int NumberOfElements = ContainerToSearch.size();
		int LowerLimit = -1;
		int UpperLimit = NumberOfElements;
		while (true)
		{
			int CurrentGuess = (LowerLimit + UpperLimit) / 2;
			if (CompareFunction(ContainerToSearch[CurrentGuess],ValueToGet))
			{
				LowerLimit = CurrentGuess;
			}
			else
			{
				if (ContainerToSearch[CurrentGuess] == ValueToGet)
				{
					ReturnValue = CurrentGuess;
					break;
				}
				UpperLimit = CurrentGuess;
			}
			if (UpperLimit - LowerLimit == 1)
			{
				if (ContainerToSearch[LowerLimit] == ValueToGet)
				{
					ReturnValue = LowerLimit;
				}
				break;
			}
		}
		return(ReturnValue);
	}
	template<typename Container, typename Value,typename GetFunc, typename Compare> int BinarySearch(Container const& ContainerToSearch, Value const& ValueToGet,GetFunc ContainerGetFunction, Compare CompareFunction)
	{
		int ReturnValue = -1;
		int NumberOfElements = ContainerToSearch.size();
		int LowerLimit = -1;
		int UpperLimit = NumberOfElements;
		if (NumberOfElements == 0)
		{
			return(-1);
		}
		while (true)
		{
			int CurrentGuess = (LowerLimit + UpperLimit) / 2;
			if (CompareFunction(ContainerGetFunction(ContainerToSearch,CurrentGuess), ValueToGet))
			{
				LowerLimit = CurrentGuess;
			}
			else
			{
				if (ContainerGetFunction(ContainerToSearch,CurrentGuess) == ValueToGet)
				{
					ReturnValue = CurrentGuess;
					break;
				}
				UpperLimit = CurrentGuess;
			}
			if (UpperLimit - LowerLimit == 1)
			{
				if (ContainerGetFunction(ContainerToSearch, CurrentGuess) == ValueToGet)
				{
					ReturnValue = LowerLimit;
				}
				break;
			}
		}
		return(ReturnValue);
	}
}