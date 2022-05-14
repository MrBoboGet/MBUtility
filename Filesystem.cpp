#include "Filesystem.h"
#include <map>
#include <filesystem>
#include <codecvt>
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
		std::string ReturnValue = i_PathToUTF8(std::filesystem::canonical(m_CurrentDirectory));
		return(ReturnValue);
	}
	FilesystemError OS_Filesystem::ChangeDirectory(std::string const& NewDirectory)
	{
		FilesystemError ReturnValue = FilesystemError::Ok;
		std::error_code Error;
		std::filesystem::path NewPath = m_CurrentDirectory / NewDirectory;
		if (!std::filesystem::exists(NewPath) || !std::filesystem::is_directory(NewPath))
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
	FSObjectInfo OS_Filesystem::GetInfo(std::string const& Path, FilesystemError* OutError)
	{
		FSObjectInfo ReturnValue;
		std::error_code Error;
		std::filesystem::directory_entry Entry = std::filesystem::directory_entry(m_CurrentDirectory / Path, Error);
		if (Error)
		{
			*OutError = FilesystemError::Unknown;
			return(ReturnValue);
		}
		ReturnValue.Name = i_PathToUTF8(Entry.path().filename());
		ReturnValue.Size = Entry.file_size();
		ReturnValue.Type = i_STDFSTypeToMBFSType(Entry.status().type());
		ReturnValue.LastWriteTime = p_GetPathWriteTime(Entry);
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
			NewInfo.Size = Entry.file_size();
			NewInfo.Type = i_STDFSTypeToMBFSType(Entry.status().type());
			NewInfo.LastWriteTime = p_GetPathWriteTime(Entry);
			ReturnValue.push_back(std::move(NewInfo));
		}
		return(ReturnValue);
	}
	std::unique_ptr<MBUtility::MBSearchableInputStream> OS_Filesystem::GetFileInputStream(std::string const& FilePath, FilesystemError* OutError)
	{
		std::unique_ptr<MBUtility::MBSearchableInputStream> ReturnValue;
		std::filesystem::path AbsolutePath = m_CurrentDirectory / FilePath;
		std::error_code Error;
		if (!std::filesystem::exists(AbsolutePath, Error) || !std::filesystem::is_regular_file(AbsolutePath))
		{
			*OutError = FilesystemError::EntryDoesntExist;
			return(ReturnValue);
		}
		//TODO check for acces rights
		//TODO update MBFileInput stream to include std::filesystem path
		return(std::unique_ptr<MBUtility::MBFileInputStream>(new MBUtility::MBFileInputStream(AbsolutePath.c_str())));
	}
	std::unique_ptr<MBUtility::MBSearchableOutputStream> OS_Filesystem::GetFileOutputStream(std::string const& FilePath, FilesystemError* OutError)
	{
		std::unique_ptr<MBUtility::MBOctetOutputStream> ReturnValue;
		std::filesystem::path AbsolutePath = m_CurrentDirectory / FilePath;
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