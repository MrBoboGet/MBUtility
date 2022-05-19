#pragma once
#include "MBInterfaces.h"
#include "MBErrorHandling.h"

#include <filesystem>
namespace MBUtility
{
	enum class FileSystemType
	{
		Null,
		Directory,
		File,
	};
	std::string FSTypeToString(FileSystemType TypeToConvert);
	FileSystemType StringToFSObjectType(std::string const& TypeToConvert);
	struct FSObjectInfo
	{
		std::string Name;
		FileSystemType Type = FileSystemType::Null;
		uint64_t Size = 0;
		uint64_t LastWriteTime = 0;
	};
	enum class FilesystemError
	{
		Ok,
		EntryDoesntExist,
		InvalidFilesystemType,
		InvalidCredentials,
		Unknown,
	};
	inline bool operator!(FilesystemError Error)
	{
		return(Error != FilesystemError::Ok);
	}
	std::string FSErrorToString(FilesystemError ErrorToConvert);
	//ABSTRACT CLASSES
	class Filesystem
	{
	public:
		virtual std::string GetCurrentPath() = 0;
		virtual FilesystemError ChangeDirectory(std::string const& NewDirectory) = 0;
		virtual FSObjectInfo GetInfo(std::string const& Path, FilesystemError* OutError) = 0;
		virtual std::vector<FSObjectInfo> ListDirectory(std::string const& Path, FilesystemError* OutError) = 0;
		virtual std::unique_ptr<MBUtility::MBSearchableInputStream> GetFileInputStream(std::string const& FilePath, FilesystemError* OutError) = 0;
		virtual std::unique_ptr<MBUtility::MBSearchableOutputStream> GetFileOutputStream(std::string const& FilePath, FilesystemError* OutError) = 0;
	};

	class OS_Filesystem : public Filesystem
	{
	private:
		std::filesystem::path m_CurrentDirectory;
	public:
		OS_Filesystem(std::string const& User, std::filesystem::path StartDirectory);
		OS_Filesystem();
		//Error Open(std::string const& User, std::filesystem::path StartDirectory);?
		virtual std::string GetCurrentPath();
		virtual FilesystemError ChangeDirectory(std::string const& NewDirectory) override;
		virtual FSObjectInfo GetInfo(std::string const& Path, FilesystemError* OutError) override;
		virtual std::vector<FSObjectInfo> ListDirectory(std::string const& Path, FilesystemError* OutError) override;
		virtual std::unique_ptr<MBUtility::MBSearchableInputStream> GetFileInputStream(std::string const& FilePath, FilesystemError* OutError) override;
		virtual std::unique_ptr<MBUtility::MBSearchableOutputStream> GetFileOutputStream(std::string const& FilePath, FilesystemError* OutError) override;
	};
}