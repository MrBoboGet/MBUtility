#pragma once
#include <string>
#include "MBInterfaces.h"
#include "IndeterminateInputStream.h"
namespace MBUtility
{
    inline std::string ReadAll(MBUtility::MBOctetInputStream& InStream)
    {
        std::string ReturnValue;
        constexpr size_t ReadSize = 4096;
        char Buffer[ReadSize];
        while(true)
        {
            size_t ReadBytes = InStream.Read(Buffer,ReadSize);
            ReturnValue.insert(ReturnValue.end(),Buffer,Buffer+ReadBytes);
            if(ReadBytes < ReadSize)
            {
                break;
            }
        }
        return ReturnValue;
    }
    inline std::string ReadAll(MBUtility::IndeterminateInputStream& InStream)
    {
        std::string ReturnValue;
        constexpr size_t ReadSize = 4096;
        char Buffer[ReadSize];
        while(true)
        {
            size_t ReadBytes = InStream.ReadSome(Buffer,ReadSize);
            if(ReadBytes == 0)
            {
                break;   
            }
            ReturnValue.insert(ReturnValue.end(),Buffer,Buffer+ReadBytes);
        }
        return ReturnValue;
    }
}
