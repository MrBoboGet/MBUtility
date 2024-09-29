#include "MBInterfaces.h"
#include "IndeterminateInputStream.h"
namespace MBUtility
{
    inline std::string ReadAll(MBUtility::MBOctetInputStream&  InputStream)
    {
        std::string ReturnValue;
        constexpr size_t ReadChunkSize = 4096;
        char Buffer[ReadChunkSize];
        while(true)
        {
            auto ReadBytes = InputStream.Read(Buffer,ReadChunkSize);
            ReturnValue.insert(ReturnValue.end(),Buffer,Buffer+ReadBytes);
            if(ReadBytes < ReadChunkSize)
            {
                break;   
            }
        }
        return ReturnValue;
    }
    inline std::string ReadAll(MBUtility::IndeterminateInputStream&  InputStream)
    {
        std::string ReturnValue;
        constexpr size_t ReadChunkSize = 4096;
        char Buffer[ReadChunkSize];
        while(true)
        {
            auto ReadBytes = InputStream.ReadSome(Buffer,ReadChunkSize);
            ReturnValue.insert(ReturnValue.end(),Buffer,Buffer+ReadBytes);
            if(ReadBytes == 0)
            {
                break;   
            }
        }
        return ReturnValue;
    }
}
