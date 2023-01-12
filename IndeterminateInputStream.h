#pragma once
#include <cstddef>
#include <stdexcept>
namespace MBUtility
{
    class IndeterminateInputStream
    {
    public:
        //Same semantics as recv on regular socket, blocks until atleast some
        //data is read, and returns the amount of bytes read. The maximum data
        //that can be recieved is DataSize. A returnvalue of 0 indicates that
        //the stream is finished, and subsequent reads MUST throw an exception
        //derived from std::exception.
        virtual size_t ReadSome(void* Buffer,size_t BufferSize) = 0;
    };
    inline size_t ReadExact(IndeterminateInputStream& Stream,void* Buffer,size_t BufferSize)
    {
        size_t ReturnValue = 0;
        char* ByteBuffer = (char*)Buffer;
        while(ReturnValue < BufferSize)
        {
            size_t ReadBytes = Stream.ReadSome(ByteBuffer+ReturnValue,BufferSize-ReturnValue);
            if(ReadBytes == 0)
            {
                throw std::runtime_error("Insufficient bytes int stream");
            }
            ReturnValue += ReadBytes;
        }
        return(ReturnValue);
    }
    
}
