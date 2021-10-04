#pragma once
#include <fstream>
#include <string>
#include <cstdint>
#include <algorithm>
#include <stdexcept>

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
			for (size_t i = 0; i < RequestedBytes; i++)
			{
				OutBuffer[i] = m_BufferToReadFrom[i + m_ReadOffset];
			}
			m_ReadOffset += BytesToRead;
			return(BytesToRead);
		}
		~MBBufferInputStream() override
		{

		}
	};
	class MBFileInputStream : public MBOctetInputStream
	{
	private:
		std::ifstream* m_UnderlyingHandle;
		bool m_StreamFinished = false;
	public:
		MBFileInputStream(std::ifstream* AssociatedFile)
		{
			m_UnderlyingHandle = AssociatedFile;
		}
		size_t Read(void* Buffer, size_t RequestedBytes)
		{
			if (m_StreamFinished && RequestedBytes > 0)
			{
				throw std::out_of_range("Reading beyond file size");
			}
			m_UnderlyingHandle->read((char*)Buffer, RequestedBytes);
			size_t BytesRead = m_UnderlyingHandle->gcount();
			if (BytesRead < RequestedBytes)
			{
				m_StreamFinished = true;
			}
			return(m_UnderlyingHandle->gcount());
		}
		~MBFileInputStream() override
		{

		}
	};

	class MBBasicUserAuthenticator
	{
	private:
	public:
		virtual bool VerifyUser(std::string const& Username, std::string const& Password) = 0;
	};
}