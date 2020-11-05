#pragma once
#include <string>
#include <fstream>
#include <vector>
class TextReader
{
private:
	std::ifstream DataStreamen;
	bool IsOpen = false;
	int NumberOfLines = -1;
	std::string FilePathOfFile;
	std::vector<int> LinePositions = std::vector<int>(0);
	int MaxLineLength = 1000;
public:
	TextReader(std::string FilePath);
	int Size();
	~TextReader();
	std::string operator[](int Index)
	{
		DataStreamen.clear();
		DataStreamen.seekg(LinePositions[Index],DataStreamen.beg);
		//std::cout << LinePositions[Index] << std::endl;
		//std::cout << DataStreamen.tellg() << std::endl;
		std::string Linen;
		std::getline(DataStreamen, Linen);
		return(Linen);
	}
	std::vector<std::string> GetAllLines();
};
int TextReader::Size()
{
	return(NumberOfLines);
}
std::vector<std::string> TextReader::GetAllLines()
{
	std::vector<std::string> ReturValue = std::vector<std::string>(0);
	std::string line;
	//DataStreamen.close();
	//DataStreamen.open(FilePathOfFile);
	DataStreamen.clear();
	DataStreamen.seekg(0,DataStreamen.beg);
	while (std::getline(DataStreamen, line))
	{
		ReturValue.push_back(line);
	}
	return(ReturValue);
}
TextReader::TextReader(std::string FilePath)
{
	DataStreamen = std::ifstream(FilePath);
	FilePathOfFile = FilePath;
    std::string line;
	NumberOfLines = 0;
	//positionen för första raden
	LinePositions.push_back(DataStreamen.tellg());
    while (std::getline(DataStreamen, line))
    {
		NumberOfLines += 1;
		LinePositions.push_back(DataStreamen.tellg());
    }
	IsOpen = true;
}

TextReader::~TextReader()
{
	DataStreamen.close();
}