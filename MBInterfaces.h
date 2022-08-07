#pragma once
#include <fstream>
#include <ostream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <stdexcept>
#include <memory>
#include <cstring>

#include "MBCompileDefinitions.h"

#if MBCppVersion >= 16
#include <filesystem>
#endif // 

namespace MBUtility
{
	class MBOctetInputStream
	{
	private:
	public:
		//Returns the amount of bytes read, may block if data isn't available
		virtual size_t Read(void* Buffer, size_t BytesToRead)
		{
			return(0);
		}
		//virtual bool DataAvailable();
		virtual ~MBOctetInputStream()
		{

		}
	};
	class MBOctetOutputStream
	{
	private:

	public:
		virtual size_t Write(const void* DataToWrite, size_t DataToWriteSize) = 0;
		virtual void Flush()
		{

		}
		virtual ~MBOctetOutputStream()
		{

		}
	};
	class MBSearchableOutputStream : public MBOctetOutputStream
	{
	public:
		virtual uintmax_t SetOutputPosition(uintmax_t NewOutputPosition) = 0;
		virtual uintmax_t GetOutputPosition() = 0;
	};










	/*
	SEEK_SET
	SEEK_CUR
	SEEK_END
	*/
	class MBSearchableInputStream : public MBOctetInputStream
	{
	public:
		virtual uint64_t SetInputPosition(int64_t Offset,int whence = SEEK_SET) = 0;
		virtual uint64_t GetInputPosition() = 0;
	};
	class MBFileOutputStream : public MBSearchableOutputStream
	{
	private:
		bool m_UseReference = false;
		std::ofstream m_FileOutput;
		std::ofstream* m_ReferenceOutput;

		std::ofstream* p_GetOutputStream()
		{
			if (m_UseReference)
			{
				return(m_ReferenceOutput);
			}
			else
			{
				return(&m_FileOutput);
			}
		}
	public:
		MBFileOutputStream(std::ofstream* OutputStream)
		{
			m_ReferenceOutput = OutputStream;
			m_UseReference = true;
		}
#if MBCppVersion >= 16
		MBFileOutputStream(std::filesystem::path const& Path)
		{
			//m_StreamObject = std::ifstream(Path.c_str(), std::ios::binary | std::ios::in);
			m_FileOutput = std::ofstream(Path.c_str(), std::ios::binary | std::ios::out);
		}
#endif // 
		void Flush() override
		{
			std::ofstream* AssociatedFile = p_GetOutputStream();
			AssociatedFile->flush();
		}
		size_t Write(const void* DataToWrite, size_t DataToWriteSize) override
		{
			std::ofstream* AssociatedFile = p_GetOutputStream();
			AssociatedFile->write((const char*)DataToWrite, DataToWriteSize);
			return(DataToWriteSize);
		}
		uintmax_t SetOutputPosition(uintmax_t NewOutputPosition) override
		{
			std::ofstream* AssociatedFile = p_GetOutputStream();
			AssociatedFile->seekp(NewOutputPosition);
			return(0);
		}
		uintmax_t GetOutputPosition() override
		{
			std::ofstream* AssociatedFile = p_GetOutputStream();
			uintmax_t ReturnValue = AssociatedFile->tellp();
			return(ReturnValue);
		}
		~MBFileOutputStream() override
		{

		}
	};
	class MBFileInputStream : public MBSearchableInputStream
	{
	private:

		std::ifstream* m_ExternalFileStream = nullptr;
		std::unique_ptr<std::ifstream> m_OwnedPointer = nullptr;
		std::ifstream m_StreamObject;

		std::ifstream* p_GetPointerToUse()
		{
			if (m_ExternalFileStream != nullptr)
			{
				return(m_ExternalFileStream);
			}
			else if (m_OwnedPointer != nullptr)
			{
				return(m_OwnedPointer.get());
			}
			else if (m_StreamObject.is_open())
			{
				return(&m_StreamObject);
			}
			return(nullptr);
		}
	public:
		MBFileInputStream(std::ifstream* OutputStream)
		{
			m_ExternalFileStream = OutputStream;
		}
		MBFileInputStream(std::string const& FilePath)
		{
			m_StreamObject = std::ifstream(FilePath, std::ios::binary | std::ios::in);
		}
#if MBCppVersion >= 16
		MBFileInputStream(std::filesystem::path const& Path)
		{
			m_StreamObject = std::ifstream(Path.c_str(), std::ios::binary | std::ios::in);
		}
#endif // 


		MBFileInputStream(std::ifstream&& FileToSteal)
		{
			m_StreamObject = std::move(FileToSteal);
		}
		size_t Read(void* Buffer, size_t BytesToRead) override
		{
			std::ifstream* PointerToUse = p_GetPointerToUse();
			if (PointerToUse == nullptr)
			{
				return(0);
			}
			else
			{
				size_t ReturnValue = PointerToUse->read((char*)Buffer, BytesToRead).gcount();
				if (PointerToUse->eof())
				{
					PointerToUse->clear();
				}
				return(ReturnValue);
			}
		}
		uint64_t SetInputPosition(int64_t NewOutputPosition,int whence) override
		{
			std::ifstream* PointerToUse = p_GetPointerToUse();
			if (PointerToUse == nullptr)
			{
				return(-1);
			}
			uintmax_t ReturnValue = -1;
			if (whence == SEEK_SET)
			{
				ReturnValue = PointerToUse->seekg(NewOutputPosition).tellg();
			}
			else if (whence == SEEK_CUR)
			{
				ReturnValue = PointerToUse->seekg(NewOutputPosition, std::ios::cur).tellg();
			}
			else if (whence == SEEK_END)
			{
				ReturnValue = PointerToUse->seekg(NewOutputPosition, std::ios::end).tellg();
			}
			return(ReturnValue);
		}
		uint64_t GetInputPosition() override
		{
			std::ifstream* PointerToUse = p_GetPointerToUse();
			if (PointerToUse == nullptr)
			{
				return(-1);
			}
			return(PointerToUse->tellg());
		}
		~MBFileInputStream() override
		{

		}
	};
	class MBStringOutputStream : public MBSearchableOutputStream
	{
	private:
		std::string* m_OutputString = nullptr;
		size_t m_OutputPosition = 0;
	public:
		MBStringOutputStream(std::string& OutputString)
		{
			m_OutputString = &OutputString;
		}
		size_t Write(const void* DataToWrite, size_t DataToWriteSize) override
		{
			if (m_OutputPosition + DataToWriteSize > m_OutputString->size())
			{
				m_OutputString->resize(m_OutputPosition + DataToWriteSize, 0);
			}
			std::memcpy((void*)(m_OutputString->data() + m_OutputPosition), DataToWrite, DataToWriteSize);
			m_OutputPosition += DataToWriteSize;
			return(DataToWriteSize);
		}
		uintmax_t SetOutputPosition(uintmax_t NewOutputPosition) override
		{
			m_OutputPosition = NewOutputPosition;
			return(NewOutputPosition);
		}
		uintmax_t GetOutputPosition() override
		{
			return(m_OutputPosition);
		}
		~MBStringOutputStream() override
		{

		}
	};
	class MBBufferInputStream : public MBOctetInputStream
	{
	private:
		const uint8_t* m_BufferToReadFrom = nullptr;
		size_t m_BufferSize = 0;

		size_t m_ReadOffset = 0;
	public:
		MBBufferInputStream(const void* DataToRead, size_t DataSize)
		{
			m_BufferToReadFrom = (uint8_t*)DataToRead;
			m_BufferSize = DataSize;
		}
		size_t Read(void* Buffer, size_t RequestedBytes) override
		{
			size_t BytesToRead = std::min(RequestedBytes, m_BufferSize - m_ReadOffset);
			uint8_t* OutBuffer = (uint8_t*)Buffer;
			//for (size_t i = 0; i < BytesToRead; i++)
			//{
			//	OutBuffer[i] = m_BufferToReadFrom[i + m_ReadOffset];
			//}
			std::memcpy(Buffer, m_BufferToReadFrom + m_ReadOffset, BytesToRead);
			m_ReadOffset += BytesToRead;
			return(BytesToRead);
		}
		~MBBufferInputStream() override
		{

		}
	};
	//class MBFileInputStream : public MBOctetInputStream
	//{
	//private:
	//	std::ifstream* m_UnderlyingHandle;
	//	bool m_StreamFinished = false;
	//public:
	//	MBFileInputStream(std::ifstream* AssociatedFile)
	//	{
	//		m_UnderlyingHandle = AssociatedFile;
	//	}
	//	size_t Read(void* Buffer, size_t RequestedBytes)
	//	{
	//		if (m_StreamFinished && RequestedBytes > 0)
	//		{
	//			throw std::out_of_range("Reading beyond file size");
	//		}
	//		m_UnderlyingHandle->read((char*)Buffer, RequestedBytes);
	//		size_t BytesRead = m_UnderlyingHandle->gcount();
	//		if (BytesRead < RequestedBytes)
	//		{
	//			m_StreamFinished = true;
	//		}
	//		return(m_UnderlyingHandle->gcount());
	//	}
	//	~MBFileInputStream() override
	//	{
	//
	//	}
	//};

	class MBBasicUserAuthenticator
	{
	private:
	public:
		virtual bool VerifyUser(std::string const& Username, std::string const& Password) = 0;
	};

	inline void WriteData(MBOctetInputStream& Input,MBOctetOutputStream& Output)
	{
		constexpr size_t ReadChunkSize = 4096;
		uint8_t Buffer[ReadChunkSize];
		while (true)
		{
			size_t ReadBytes = Input.Read(Buffer, ReadChunkSize);
			Output.Write(Buffer, ReadBytes);
			if (ReadBytes < ReadChunkSize)
			{
				break;
			}
		}
		Output.Flush();
	}

    class LineRetriever
    {
        private:
            MBUtility::MBOctetInputStream* m_InputStream = nullptr;
            std::string m_CurrentLine;
            std::string m_Buffer;
            size_t m_BufferOffset = 0;
            bool m_Finished = false;
            bool m_StreamFinished = false;
            bool m_LineBuffered = false;
        public:
            LineRetriever(MBUtility::MBOctetInputStream* InputStream);
            bool Finished();
            bool GetLine(std::string& OutLine);
            void DiscardLine();
            std::string& PeekLine();

    };


}
