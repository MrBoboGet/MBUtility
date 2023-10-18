#pragma once
#include <string>
#include "IndeterminateInputStream.h"
#include <memory>
#include <cstring>

#include "MBInterfaces.h"
namespace MBUtility
{

    class IndeterminateStringStream : public IndeterminateInputStream
    {
        std::string_view m_Data;
        size_t m_ParseOffset = 0;
    public:
        IndeterminateStringStream(std::string_view Data)
        {
            m_Data = Data;
        }
        virtual size_t ReadSome(void* Buffer,size_t BufferSize)
        {
            size_t ReturnValue = std::min(m_Data.size()-m_ParseOffset,BufferSize);
            std::memcpy(Buffer,m_Data.data()+m_ParseOffset,ReturnValue);
            m_ParseOffset += ReturnValue;
            return ReturnValue;
        }
    };
    class OwningStringStream : public IndeterminateInputStream
    {
        std::string m_Data;
        size_t m_ParseOffset = 0;
    public:
        OwningStringStream(std::string_view Data)
        {
            m_Data = Data;
        }
        virtual size_t ReadSome(void* Buffer,size_t BufferSize)
        {
            size_t ReturnValue = std::min(m_Data.size()-m_ParseOffset,BufferSize);
            std::memcpy(Buffer,m_Data.data()+m_ParseOffset,ReturnValue);
            m_ParseOffset += ReturnValue;
            return ReturnValue;
        }
    };

    class IndeterminateInputStream_Ref : public IndeterminateInputStream
    {
        IndeterminateInputStream* m_Ref = nullptr;
    public:
        IndeterminateInputStream_Ref(IndeterminateInputStream* Ref)
        {
            m_Ref = Ref;
        }
        virtual size_t ReadSome(void* Buffer,size_t BufferSize)
        {
            return m_Ref->ReadSome(Buffer,BufferSize);
        }
    };

    
    class StreamReader : public IndeterminateInputStream, public MBOctetInputStream
    {
        std::string m_Buffer;
        size_t m_CurrentOffset = 0;
        size_t m_CurrentBufferSize = 0;
        size_t m_TotalOffset = 0;
        bool m_EOFReached = false;
        std::unique_ptr<IndeterminateInputStream> m_Stream;


        static constexpr size_t ReadSize = 4096;
        void p_FillBuffer()
        {
            m_TotalOffset += m_CurrentBufferSize;
            size_t ReadBytes = m_Stream->ReadSome(m_Buffer.data(),ReadSize);
            if(ReadBytes == 0)
            {
                m_EOFReached = true;   
            }
            m_CurrentBufferSize = ReadBytes;
            m_CurrentOffset = 0;
        }
    public:
        StreamReader(std::unique_ptr<IndeterminateInputStream> InputStream)
        {
            m_Buffer = std::string(ReadSize,0);
            m_Stream = std::move(InputStream);
        }

        size_t Position() const
        {
            return m_TotalOffset+m_CurrentOffset;
        }
        template<typename PredType>
        void SkipWhile(PredType const& Pred)
        {
            while(!EOFReached())
            {
                if(m_CurrentOffset == m_CurrentBufferSize)
                {
                    p_FillBuffer();
                    continue;
                }
                if(Pred(m_Buffer[m_CurrentOffset]))
                {
                    m_CurrentOffset += 1;   
                }
                else
                {
                    break;   
                }
            }
        }
        template<typename PredType,typename DoType>
        void DoWhile(PredType const& Pred,DoType Doer)
        {
            while(!EOFReached())
            {
                if(m_CurrentOffset == m_CurrentBufferSize)
                {
                    p_FillBuffer();
                    continue;
                }
                if(Pred(m_Buffer[m_CurrentOffset]))
                {
                    Doer(m_Buffer[m_CurrentOffset]);
                    m_CurrentOffset += 1;   
                }
                else
                {
                    break;
                }

            }
        }
        template<typename PredType>
        std::string ReadWhile(PredType const& Pred)
        {
            std::string ReturnValue;
            while(!EOFReached())
            {
                if(m_CurrentOffset == m_CurrentBufferSize)
                {
                    p_FillBuffer();
                    continue;
                }
                if(Pred(m_Buffer[m_CurrentOffset]))
                {
                    ReturnValue += m_Buffer[m_CurrentOffset];
                    m_CurrentOffset += 1;
                }
                else
                {
                    break;   
                }
            }
            return ReturnValue;
        }
        bool EOFReached() const
        {
            return m_EOFReached;
        }

        char PeekByte()
        {
            if(EOFReached())
            {
                throw std::runtime_error("EOF end reached, cannot peek byte");
            }
            if(m_CurrentOffset == m_CurrentBufferSize)
            {
                p_FillBuffer();
            }
            if(EOFReached())
            {
                throw std::runtime_error("EOF end reached, cannot peek byte");
            }
            return m_Buffer[m_CurrentOffset];
        }
        char ReadByte()
        {
            char ReturnValue = PeekByte();
            m_CurrentOffset += 1;
            return ReturnValue;
        }
        size_t ReadSome(void* Buffer,size_t ReadSize) override
        {
            size_t ReturnValue = 0;
            if(EOFReached())
            {
                return ReturnValue;   
            }
            if(m_CurrentBufferSize == m_CurrentOffset)
            {
                p_FillBuffer();   
                if(EOFReached()) return ReturnValue;
            }
            //assert(m_CurrentOffset < m_CurrentBufferSize)
            size_t NewReadSize = std::min(ReadSize,m_CurrentBufferSize-m_CurrentOffset);
            std::memcpy(Buffer,m_Buffer.data()+m_CurrentOffset,NewReadSize);
            m_CurrentOffset += NewReadSize;
            ReturnValue = NewReadSize;
            return ReturnValue;
        }
        size_t Read(void* Buffer,size_t ReadSize) override
        {
            return ReadExact(*this,Buffer,ReadSize);
        }
    };
    inline std::string ReadLine(MBUtility::StreamReader& Reader)
    {
        std::string ReturnValue;
        ReturnValue = Reader.ReadWhile([](char CurrentByte){return CurrentByte != '\n';});
        if(!Reader.EOFReached())
        {
            Reader.ReadByte();
        }
        if(ReturnValue.size() > 0 && ReturnValue.back() == '\r')
        {
            ReturnValue.resize(ReturnValue.size()-1);
        }
        return ReturnValue;
    }
};
