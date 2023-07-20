#include "IndeterminateInputStream.h"
#include "MBInterfaces.h"
namespace MBUtility
{
    class InputFileStream : public MBSearchableInputStream, public IndeterminateInputStream
    {
        std::ifstream m_OpenFile;
        uint64_t m_Position = 0;
    public:
        InputFileStream(std::filesystem::path const& Path) : m_OpenFile(Path,std::ios::in | std::ios::binary)
        {
        }
		size_t Read(void* Buffer, size_t BytesToRead) override
		{
            size_t ReturnValue = m_OpenFile.read((char*)Buffer, BytesToRead).gcount();
            if (m_OpenFile.eof())
            {
                m_OpenFile.clear();
            }
            m_Position += ReturnValue;
            return(ReturnValue);
		}
		uint64_t SetInputPosition(int64_t NewOutputPosition,int whence) override
		{
			uintmax_t ReturnValue = -1;
			if (whence == SEEK_SET)
			{
				ReturnValue = m_OpenFile.seekg(NewOutputPosition).tellg();
                m_Position = NewOutputPosition;
			}
			else if (whence == SEEK_CUR)
			{
				ReturnValue = m_OpenFile.seekg(NewOutputPosition, std::ios::cur).tellg();
                m_Position = m_Position + NewOutputPosition;
			}
			else if (whence == SEEK_END)
			{
				ReturnValue = m_OpenFile.seekg(NewOutputPosition, std::ios::end).tellg();
                m_Position = m_OpenFile.tellg();
			}
			return(ReturnValue);
		}
		uint64_t GetInputPosition() override
		{
			return(m_Position);
		}
        virtual size_t ReadSome(void* Buffer,size_t BufferSize) override
        {
            return Read(Buffer,BufferSize);
        }
    };
}
