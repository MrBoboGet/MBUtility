#include "Filesystem.h"
#include <map>
#include <filesystem>
#include <codecvt>

#ifdef MBPosix
#define _FILE_OFFSET_BITS = 64
#include <sys/stat.h>
#endif // 


namespace MBUtility
{
	std::string i_PathToUTF8(std::filesystem::path const& PathToConvert)
	{
		std::string ReturnValue;
#ifdef MBWindows
		std::u16string source = std::u16string((char16_t*)PathToConvert.c_str());
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;
		ReturnValue = convert.to_bytes(source);
#else
		return(PathToConvert.generic_u8string());
#endif
		return(ReturnValue);
	}

	uint64_t i_GetModificationTime(std::filesystem::path const& PathToStat)
	{
#ifdef MBWindows
		return(std::filesystem::last_write_time(PathToStat).time_since_epoch().count());
#else
		struct stat64 FileStats;
		stat64(PathToStat.c_str(), &FileStats);
		//std::cout << size_t(FileStats.st_size) << std::endl;
		return(FileStats.st_mtime);
#endif
	}
	uint64_t i_GetFilesize(std::filesystem::path const& PathToCheck)
	{
#ifdef MBWindows
		return(std::filesystem::file_size(PathToCheck));
#else
		struct stat64 FileStats;
		stat64(PathToCheck.c_str(), &FileStats);
		//std::cout << size_t(FileStats.st_size) << std::endl;
		return(FileStats.st_size);
#endif

	}

	const char* i_FSErrorTypeToStringMap[] =
	{
		"Ok",
		"Entry doesn't exist",
		"Invalid Filesystem type",
		"Invalid Filesystem type",
		"Unknown"
	};
	std::string FSErrorToString(FilesystemError ErrorToConvert)
	{
		if (uint32_t(ErrorToConvert) >= sizeof(i_FSErrorTypeToStringMap) / sizeof(const char*))
		{
			throw std::runtime_error("Invalid FilesystemType");
		}
		return(i_FSErrorTypeToStringMap[size_t(ErrorToConvert)]);
	}

	FileSystemType i_STDFSTypeToMBFSType(std::filesystem::file_type TypeToConvert)
	{
		FileSystemType ReturnValue = FileSystemType::Null;
		if (TypeToConvert == std::filesystem::file_type::directory)
		{
			ReturnValue = FileSystemType::Directory;
		}
		else if (TypeToConvert == std::filesystem::file_type::regular)
		{
			ReturnValue = FileSystemType::File;
		}
		//TODO *turbo hack*. std::filesystem::file_type is for some reasons unknown for files larger than 4gig...
		else if (TypeToConvert == std::filesystem::file_type::unknown)
		{
			ReturnValue = FileSystemType::File;
		}
		else
		{
			//mainly debugging
			throw std::runtime_error("Invalid filesystem type");
		}
		return(ReturnValue);
	}
	OS_Filesystem::OS_Filesystem()
	{
		m_CurrentDirectory = std::filesystem::current_path();
	}
	OS_Filesystem::OS_Filesystem(std::string const& User, std::filesystem::path StartDirectory)
	{
		m_CurrentDirectory = std::move(StartDirectory);
	}
	std::string OS_Filesystem::GetCurrentPath()
	{
		std::string ReturnValue = i_PathToUTF8(m_CurrentDirectory.lexically_normal());
		return(ReturnValue);
	}
	FilesystemError OS_Filesystem::ChangeDirectory(std::string const& NewDirectory)
	{
		FilesystemError ReturnValue = FilesystemError::Ok;
		std::error_code Error;
		std::filesystem::path NewPath = (m_CurrentDirectory / NewDirectory).lexically_normal();
		if (!std::filesystem::exists(NewPath,Error) || !std::filesystem::is_directory(NewPath,Error))
		{
			return FilesystemError::EntryDoesntExist;
		}

		m_CurrentDirectory = NewPath;
		return(ReturnValue);
	}
	uint64_t p_GetPathWriteTime(std::filesystem::directory_entry const& Entry)
	{
		return(Entry.last_write_time().time_since_epoch().count());
	}
	FSObjectInfo OS_Filesystem::GetInfo(std::string const& InPath, FilesystemError* OutError)
	{
		FSObjectInfo ReturnValue;
        std::filesystem::path Path = m_CurrentDirectory/InPath;
		if (!std::filesystem::exists(Path))
		{
			*OutError = FilesystemError::EntryDoesntExist;
			return(ReturnValue);
		}
		//std::error_code Error;
		//std::filesystem::directory_entry Entry = std::filesystem::directory_entry(m_CurrentDirectory / Path, Error);
		//if (Error)
		//{
		//	*OutError = FilesystemError::Unknown;
		//	return(ReturnValue);
		//}
		ReturnValue.Name = i_PathToUTF8(std::filesystem::path(Path).filename());
		if (std::filesystem::status(Path).type() == std::filesystem::file_type::regular || std::filesystem::status(Path).type() == std::filesystem::file_type::unknown)
		{
			ReturnValue.Size = i_GetFilesize(Path); //Entry.file_size();
		}
		ReturnValue.Type = i_STDFSTypeToMBFSType(std::filesystem::status(Path).type());
		ReturnValue.LastWriteTime = i_GetModificationTime(Path); //p_GetPathWriteTime(Entry);
		return(ReturnValue);
	}
	std::vector<FSObjectInfo> OS_Filesystem::ListDirectory(std::string const& Path, FilesystemError* OutError)
	{
		std::vector<FSObjectInfo> ReturnValue;
		std::filesystem::path PathToList = m_CurrentDirectory / Path;
		if (!std::filesystem::exists(PathToList))
		{
			*OutError = FilesystemError::EntryDoesntExist;
			return(ReturnValue);
		}
		if (!std::filesystem::directory_entry(PathToList).is_directory())
		{
			*OutError = FilesystemError::InvalidFilesystemType;
			return(ReturnValue);
		}
		std::error_code OSResult;
		std::filesystem::directory_iterator Iterator(PathToList, OSResult);
		if (OSResult)
		{
			*OutError = FilesystemError::Unknown;
			return(ReturnValue);
		}
		for (auto& Entry : Iterator)
		{
			FSObjectInfo NewInfo;
			NewInfo.Name = i_PathToUTF8(Entry.path().filename());
			if (Entry.is_regular_file() || std::filesystem::status(Entry.path()).type() == std::filesystem::file_type::unknown)
			{
				NewInfo.Size = i_GetFilesize(Entry.path()); //Entry.file_size();
			}
			NewInfo.Type = i_STDFSTypeToMBFSType(Entry.status().type());
			NewInfo.LastWriteTime = i_GetModificationTime(Entry.path());//p_GetPathWriteTime(Entry);
			ReturnValue.push_back(std::move(NewInfo));
		}
		return(ReturnValue);
	}
	FilesystemError OS_Filesystem::Exists(std::string const& Path)
	{
		FilesystemError ReturnValue = FilesystemError::Ok;
		std::error_code OSError;
		if (!std::filesystem::exists(m_CurrentDirectory/Path,OSError))
		{
			ReturnValue = FilesystemError::EntryDoesntExist;
		}
		return(ReturnValue);
	}
	std::unique_ptr<MBUtility::MBSearchableInputStream> OS_Filesystem::GetFileInputStream(std::string const& FilePath, FilesystemError* OutError)
	{
		std::unique_ptr<MBUtility::MBSearchableInputStream> ReturnValue;
		std::filesystem::path AbsolutePath = m_CurrentDirectory / FilePath;
		std::error_code Error;
		if (!std::filesystem::exists(AbsolutePath, Error) || (!std::filesystem::is_regular_file(AbsolutePath) && std::filesystem::status(AbsolutePath).type() != std::filesystem::file_type::unknown))
		{
			*OutError = FilesystemError::EntryDoesntExist;
			return(ReturnValue);
		}
		//TODO check for acces rights
		//TODO update MBFileInput stream to include std::filesystem path
		return(std::unique_ptr<MBUtility::MBFileInputStream>(new MBUtility::MBFileInputStream(AbsolutePath)));
	}
	std::unique_ptr<MBUtility::MBSearchableOutputStream> OS_Filesystem::GetFileOutputStream(std::string const& FilePath, FilesystemError* OutError)
	{
		std::unique_ptr<MBUtility::MBOctetOutputStream> ReturnValue;
		std::filesystem::path AbsolutePath = m_CurrentDirectory / FilePath;
		if (!std::filesystem::exists(AbsolutePath.parent_path()))
		{
			std::filesystem::create_directories(AbsolutePath.parent_path());
		}
		//TODO check for acces rights
		//TODO update MBFileInput stream to include std::filesystem path
		return(std::unique_ptr<MBUtility::MBFileOutputStream>(new MBUtility::MBFileOutputStream(AbsolutePath.c_str())));
	}
	const char* i_InternalFSToStringMap[] = { "Null","Directory","File" };
	std::string FSTypeToString(FileSystemType TypeToConvert)
	{
		std::string ReturnValue;
		uint64_t TypeValue = uint64_t(TypeToConvert);
		if (TypeValue > sizeof(i_InternalFSToStringMap) / sizeof(const char*))
		{
			throw std::runtime_error("Invalid FSType specified");
		}
		ReturnValue = i_InternalFSToStringMap[TypeValue];
		return(ReturnValue);
	}
	//ANTAGANDE thread safe
	const std::map<std::string, FileSystemType> i_InternalStringToFSMap = { {"Null",FileSystemType::Null},{"Directory",FileSystemType::Directory},{"File",FileSystemType::File} };
	FileSystemType StringToFSObjectType(std::string const& TypeToConvert)
	{
		FileSystemType ReturnValue;
		if (i_InternalStringToFSMap.find(TypeToConvert) == i_InternalStringToFSMap.end())
		{
			throw std::runtime_error("Invalid FilesystemTypeString provided");
		}
		ReturnValue = i_InternalStringToFSMap.at(TypeToConvert);
		return(ReturnValue);
	}
};
