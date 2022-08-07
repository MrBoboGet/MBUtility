#pragma once
#include <string>
#include "MBCompileDefinitions.h"


namespace MBUtility
{
	std::string ReadWholeFile(std::string const& Filepath);
#if MBCppVersion >= 17
    //std::string ReadWholeFile(std::filesystem::path const& Path);
#endif
};
