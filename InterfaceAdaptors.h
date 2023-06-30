#pragma once
#include "MBInterfaces.h"
#include "IndeterminateInputStream.h"
namespace MBUtility
{
    class NonOwningInputStream : public MBOctetInputStream
    {
    private:
        MBOctetInputStream* m_AssociatedStream = nullptr;
    public:    
        NonOwningInputStream(MBOctetInputStream* AssociatedInputStream)
        {
            m_AssociatedStream = AssociatedInputStream; 
        }
        size_t Read(void* Data,size_t DataSize) override
        {
            return(m_AssociatedStream->Read(Data,DataSize));
        }
    };
    class NonOwningOutputStream : public MBOctetOutputStream
    {
    private:
        MBOctetOutputStream* m_AssociatedStream = nullptr;
    public:    
        NonOwningOutputStream(MBOctetOutputStream* AssociatedInputStream)
        {
            m_AssociatedStream = AssociatedInputStream; 
        }
        size_t Write(const void* Data,size_t DataSize) override
        {
            return(m_AssociatedStream->Write(Data,DataSize));
        }
    };
    class NonOwningIndeterminateInputStream : public IndeterminateInputStream
    {
        private:
            IndeterminateInputStream* m_AssociatedStream = nullptr;
        public:    
            NonOwningIndeterminateInputStream(IndeterminateInputStream* AssociatedInputStream)
            {
                m_AssociatedStream = AssociatedInputStream; 
            }
            size_t ReadSome(void* Data,size_t DataSize) override
            {
                return(m_AssociatedStream->ReadSome(Data,DataSize));
            }
    };

    class NO_IndeterminateToRegular : public MBOctetInputStream
    {
    private:
        IndeterminateInputStream* m_AssociatedStream = nullptr;
    public:
        NO_IndeterminateToRegular(IndeterminateInputStream* AssociatedInputStream)
        {
            m_AssociatedStream = AssociatedInputStream;
        }
        size_t Read(void* Buffer,size_t BufferSize)
        {
            return(ReadExact(*m_AssociatedStream,Buffer,BufferSize));
        };
    };
}
