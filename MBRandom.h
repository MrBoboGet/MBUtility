#pragma once
#include <string>
#define _CRT_RAND_S
#if defined(WIN32) || defined(_WIN32)
#include <stdlib.h>  
#elif __linux__
#include <sys/random.h>
#endif
namespace MBRandom
{
#if defined(WIN32) || defined(_WIN32)
	inline char GetRandomByte()
	{
		unsigned int RandomNumber = 0;
		errno_t ErrorCode = rand_s(&RandomNumber);
		return(RandomNumber % 256);
	}
#elif __linux__
	inline char GetRandomByte()
	{
		unsigned char RandomChar = 0;
		getrandom(&RandomChar, 1, 0);
		return(RandomChar);
	}
#endif
	inline std::string GetRandomBytes(int NumberOfBytes)
	{
		std::string ReturnValue = "";
		for (int i = 0; i < NumberOfBytes; i++)
		{
			ReturnValue += GetRandomByte();
		}
		return(ReturnValue);
	}
};
