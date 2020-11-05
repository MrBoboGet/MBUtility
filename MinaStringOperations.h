#pragma once
#include<vector>
#include <string>
#include <unordered_map>
inline std::vector<std::string> Split(std::string UrsprungsStringen, std::string SplitStringen)
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
inline void ReplaceAll(std::string* Stringen, std::string StringToReplace, std::string StringToReplaceWith)
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
		ReturnValue += CharMap[uint8_t(StringToConvert[i])%16];
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