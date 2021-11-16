#include "MBFiles.h"
#include <iostream>
#include <fstream>
#include <filesystem>
namespace MBUtility
{
	std::string ReadWholeFile(std::string const& Filepath)
	{
		size_t FileSize = std::filesystem::file_size(Filepath);
		std::string ReturnValue = std::string(FileSize, 0);
		std::ifstream FileToRead = std::ifstream(Filepath, std::ios::in | std::ios::binary);
		FileToRead.read(ReturnValue.data(), FileSize);
		return(ReturnValue);
	}
};