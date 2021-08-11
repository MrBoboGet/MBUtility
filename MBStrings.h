#define NOMINMAX
#define _CRT_RAND_S
#pragma once
#include<vector>
#include <string>
#include <unordered_map>
namespace MBUtility
{
	inline std::vector<std::string> Split(std::string const& UrsprungsStringen, std::string const& SplitStringen)
	{
		std::vector<std::string> Result = std::vector<std::string>(0);
		int StartCharacter = 0;
		size_t NuvarandeSplit = 0;
		NuvarandeSplit = UrsprungsStringen.find(SplitStringen, StartCharacter);
		while (NuvarandeSplit != UrsprungsStringen.npos)
		{
			Result.push_back(UrsprungsStringen.substr(StartCharacter, NuvarandeSplit - StartCharacter));
			StartCharacter = NuvarandeSplit + SplitStringen.size();
			NuvarandeSplit = UrsprungsStringen.find(SplitStringen, StartCharacter);
		}
		Result.push_back(UrsprungsStringen.substr(StartCharacter));
		return(Result);
	}
	inline void ReplaceAll(std::string* Stringen, std::string const& StringToReplace, std::string const& StringToReplaceWith)
	{
		size_t index = 0;
		while (true) {
			/* Locate the substring to replace. */
			index = Stringen->find(StringToReplace, index);
			if (index == std::string::npos)
			{
				break;
			}
			/* Make the replacement. */
			Stringen->replace(index, StringToReplace.size(), StringToReplaceWith);

			/* Advance index forward so the next iteration doesn't pick it up as well. */
			index += StringToReplaceWith.size();
		}
	}
	inline std::string ReplaceAll(std::string Stringen, std::string StringToReplace, std::string StringToReplaceWith)
	{
		size_t index = 0;
		while (true) {
			/* Locate the substring to replace. */
			index = Stringen.find(StringToReplace, index);
			if (index == std::string::npos)
			{
				break;
			}
			/* Make the replacement. */
			Stringen.replace(index, StringToReplace.size(), StringToReplaceWith);

			/* Advance index forward so the next iteration doesn't pick it up as well. */
			index += StringToReplaceWith.size();
		}
		return(Stringen);
	}
	inline std::string RemoveLeadingString(std::string const& StringToParse, std::string const& StringToRemove)
	{
		std::string ReturnValue = "";
		size_t ParseOffset = 0;
		if (StringToParse.find(StringToRemove) != 0)
		{
			return(StringToParse);
		}
		while (ParseOffset < StringToParse.size())
		{
			size_t NewParseOffset = StringToParse.find(StringToRemove,ParseOffset+StringToRemove.size());
			if (NewParseOffset == StringToParse.npos)
			{
				ReturnValue += (StringToParse.substr(ParseOffset+StringToRemove.size()));
				break;
			}
			else if(NewParseOffset == ParseOffset+StringToRemove.size())
			{
				ParseOffset += NewParseOffset;
			}
			else
			{
				//nästa string är inte direkt från förra, vi adderar restesn
				ReturnValue += StringToParse.substr(ParseOffset+StringToRemove.size());
				break;
			}
		}
		return(ReturnValue);
	}
	inline std::string RemoveDuplicates(std::string const& StringToParse, std::string const& DuplicateString)
	{
		std::string ReturnValue = "";
		size_t ParseOffset = StringToParse.find(DuplicateString);
		ReturnValue += StringToParse.substr(0, ParseOffset);
		while (ParseOffset < StringToParse.size())
		{
			size_t NextTargetString = StringToParse.find(DuplicateString, ParseOffset+DuplicateString.size());
			if (NextTargetString == StringToParse.npos)
			{
				ReturnValue += StringToParse.substr(ParseOffset);
				break;
			}
			if (NextTargetString == ParseOffset + DuplicateString.size())
			{
				//den är precis bredvid
				ParseOffset = NextTargetString;
				continue;
			}
			else
			{
				ReturnValue += StringToParse.substr(ParseOffset, NextTargetString - ParseOffset);
				ParseOffset = NextTargetString;
			}
		}
		return(ReturnValue);
	}
	inline std::string HexEncodeString(std::string StringToConvert)
	{
		std::string ReturnValue = "";
		std::unordered_map<uint8_t, char> CharMap(0);
		CharMap[0] = '0';
		CharMap[1] = '1';
		CharMap[2] = '2';
		CharMap[3] = '3';
		CharMap[4] = '4';
		CharMap[5] = '5';
		CharMap[6] = '6';
		CharMap[7] = '7';
		CharMap[8] = '8';
		CharMap[9] = '9';
		CharMap[10] = 'a';
		CharMap[11] = 'b';
		CharMap[12] = 'c';
		CharMap[13] = 'd';
		CharMap[14] = 'e';
		CharMap[15] = 'f';
		for (size_t i = 0; i < StringToConvert.size(); i++)
		{
			ReturnValue += CharMap[uint8_t(StringToConvert[i]) >> 4];
			ReturnValue += CharMap[uint8_t(StringToConvert[i]) % 16];
			ReturnValue += " ";
		}
		return(ReturnValue);
	}
	inline std::string HexEncodeByte(uint8_t ByteToEncode)
	{
		std::string ReturnValue = "";
		std::unordered_map<uint8_t, char> CharMap(0);
		CharMap[0] = '0';
		CharMap[1] = '1';
		CharMap[2] = '2';
		CharMap[3] = '3';
		CharMap[4] = '4';
		CharMap[5] = '5';
		CharMap[6] = '6';
		CharMap[7] = '7';
		CharMap[8] = '8';
		CharMap[9] = '9';
		CharMap[10] = 'a';
		CharMap[11] = 'b';
		CharMap[12] = 'c';
		CharMap[13] = 'd';
		CharMap[14] = 'e';
		CharMap[15] = 'f';
		ReturnValue += CharMap[ByteToEncode >> 4];
		ReturnValue += CharMap[ByteToEncode % 16];
		return(ReturnValue);
	}
	inline std::string HexEncodeInt(uint32_t IntToEncode)
	{
		std::string ReturnValue = "";
		for (int i = 3; i >= 0; i--)
		{
			ReturnValue += HexEncodeByte(IntToEncode >> (8 * i));
		}
		return(ReturnValue);
	}
	inline char HexValueToByte(char FirstCharacter, char SecondCharacter,bool* OutError)
	{
		char ReturnValue = 0;
		unsigned char Characters[2] = { FirstCharacter,SecondCharacter };
		for (size_t i = 0; i < 2; i++)
		{
			if (Characters[i] >= 71)
			{
				Characters[i] -= 32;
			}
			if (Characters[i] < 48 || (Characters[i] > 57 && Characters[i] < 65))
			{
				*OutError = false;
				return(ReturnValue);
			}
			if (Characters[i] > 70)
			{
				*OutError = false;
				return(ReturnValue);
			}
		}
		for (size_t i = 0; i < 2; i++)
		{
			if (Characters[i] < 65)
			{
				ReturnValue += ((Characters[i] - 48) << ((1 - i) * 4));
			}
			else
			{
				ReturnValue += ((Characters[i] - 55) << ((1 - i) * 4));
			}
		}
		return(ReturnValue);
	}
	inline char HexValueToByte(std::string const& StringToConvert, bool* OutError)
	{
		char ReturnValue = 0;
		if (StringToConvert.size() != 2)
		{
			*OutError = false;
			return(ReturnValue);
		}
		return(HexValueToByte(StringToConvert[0], StringToConvert[1], OutError));
	}
	inline std::string URLEncodeData(std::string const& DataToEncode)
	{
		std::string ReturnValue = "";
		for (size_t i = 0; i < DataToEncode.size(); i++)
		{
			ReturnValue += "%" + HexEncodeByte(DataToEncode[i]);
		}
		return(ReturnValue);
	}
	inline bool CharIsNumeric(char CharToCheck)
	{
		return(CharToCheck >= 48 && CharToCheck <= 57);
	}
	inline bool CharIsAlphabetical(char CharToCheck)
	{
		return((CharToCheck >= 65 && CharToCheck <= 90) || (CharToCheck >= 97 && CharToCheck <= 122));
	}
	inline bool CharIsControl(char CharToCheck)
	{
		return(CharToCheck >= 0 && CharToCheck < 32);
	}
	inline std::string URLDecodeData(std::string const& DataToDecode,bool* OutError)
	{
		std::string ReturnValue = "";
		ReturnValue.reserve(DataToDecode.size());
		size_t ParseOffset = 0;
		while (ParseOffset < DataToDecode.size())
		{
			char CurrentChar = DataToDecode[ParseOffset];
			if (CurrentChar == '+')
			{
				ParseOffset += 1;
				ReturnValue += ' ';
			}
			else if (CurrentChar == '%')
			{
				ParseOffset += 1;
				if (ParseOffset + 1 >= DataToDecode.size())
				{
					//ett error hände, vi kan inte korrekt URL decoda
					*OutError = false;
					break;
				}
				bool DecodeError = true;
				ReturnValue += HexValueToByte(DataToDecode[ParseOffset], DataToDecode[ParseOffset + 1], &DecodeError);
				if (!DecodeError)
				{
					*OutError = false;
					break;
				}
				ParseOffset += 2;
			}
			else
			{
				ParseOffset += 1;
				ReturnValue += CurrentChar;
			}
		}
		return(ReturnValue);
	}
};