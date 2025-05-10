#pragma once
#include <string>
#include "IndeterminateInputStream.h"
#include <memory>
#include <cstring>

#include "MBInterfaces.h"

#include <assert.h>

#include "Iterator.h"
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

    
    class StreamReader : public IndeterminateInputStream, virtual public MBOctetInputStream
    {
        std::string m_Buffer;
        size_t m_CurrentOffset = 0;
        size_t m_CurrentBufferSize = 0;
        uint_least64_t m_TotalOffset = 0;
        bool m_EOFReached = false;
        std::unique_ptr<IndeterminateInputStream> m_Stream;


        static constexpr size_t ReadSize = 4096;
        void p_FillBuffer()
        {
            m_TotalOffset += m_CurrentBufferSize;
            size_t ReadBytes = m_Stream->ReadSome(m_Buffer.data(),m_Buffer.size());
            if(ReadBytes == 0)
            {
                m_EOFReached = true;   
            }
            m_CurrentBufferSize = ReadBytes;
            m_CurrentOffset = 0;
        }
        void p_AddToBuffer()
        {
            assert(m_CurrentBufferSize != m_Buffer.size());
            size_t ReadBytes = m_Stream->ReadSome(m_Buffer.data()+m_CurrentBufferSize,m_Buffer.size()-m_CurrentBufferSize);
            //if(ReadBytes == 0)
            //{
            //    m_EOFReached = true;   
            //}
            m_CurrentBufferSize += ReadBytes;
        }
        void p_ExtendBuffer(size_t MinSize = 0)
        {
            size_t NewSize = m_Buffer.size();
            if(m_CurrentOffset == 0)
            {
                NewSize *= 2;
            }
            NewSize = std::max(NewSize,MinSize);
            std::string NewBuffer = std::string(NewSize,0);
            std::memcpy(NewBuffer.data(),m_Buffer.data()+m_CurrentOffset,m_Buffer.size()- m_CurrentOffset);
            m_CurrentBufferSize -= m_CurrentOffset;
            std::swap(m_Buffer,NewBuffer);
            m_CurrentOffset = 0;
        }

        class PeekIterator : public Bidir_Base<PeekIterator,const char>
        {
            friend class StreamReader;
            MBUtility::StreamReader* m_AssociatedReader = nullptr;
            bool m_Finished = false;
            size_t m_Offset = -1;

        public:
            void Increment(size_t Skip)
            {
                m_Offset += Skip;
                if(m_AssociatedReader->m_CurrentOffset+m_Offset >= m_AssociatedReader->m_CurrentBufferSize)
                {
                    if(m_AssociatedReader->m_CurrentOffset+m_Offset >= m_AssociatedReader->m_Buffer.size())
                    {
                        m_AssociatedReader->p_ExtendBuffer(m_AssociatedReader->m_CurrentOffset+m_Offset);
                    }
                    while(m_AssociatedReader->m_CurrentOffset+m_Offset >= m_AssociatedReader->m_CurrentBufferSize)
                    {
                        size_t CurrentSize = m_AssociatedReader->m_CurrentBufferSize;
                        m_AssociatedReader->p_AddToBuffer();
                        if(CurrentSize == m_AssociatedReader->m_CurrentBufferSize)
                        {
                            m_Finished = true;
                            break;
                        }
                    }
                }
            }
            typedef std::bidirectional_iterator_tag iterator_category;
            void Increment()
            {
                Increment(1);
            }
            void Decrement()
            {
                m_Offset -= 1;
                if(m_Finished)
                    m_Finished = false;
            }
            const char& GetRef() const
            {
                return m_AssociatedReader->m_Buffer[m_AssociatedReader->m_CurrentOffset+m_Offset];
            }
            bool IsEqual(PeekIterator const& rhs) const
            {
                return (m_Finished && rhs.m_Finished) || (m_Offset == rhs.m_Offset);
            }
        };
    protected:
        void p_ClearBuffer()
        {
            m_CurrentOffset = 0;
            m_CurrentBufferSize = 0;
            m_TotalOffset = 0;
        }
        void p_ClearEOF()
        {
            m_EOFReached = false;
        }
        void p_SetTotalOffset(uint_least64_t NewTotalOffset)
        {
            m_TotalOffset = NewTotalOffset;
        }
        uint_least64_t p_GetTotalOffset()
        {
            return m_TotalOffset;   
        }
        //kinda hacky
        IndeterminateInputStream* p_GetUnderlyingStream()
        {
            return m_Stream.get();
        }
    public:

        //weird interface used by tokenizer

        void Consume(PeekIterator& It,size_t ConsumedBytesCount)
        {
            if(ConsumedBytesCount > It.m_Offset)
            {
                throw std::runtime_error("Invalid ConsumedBytesCount: iterator must have atleast as large offset as the consumed bytes");
            }
            if(m_CurrentOffset+ConsumedBytesCount >= m_CurrentBufferSize)
            {
                throw std::runtime_error("Invalid consume count: larger than currently allocated buffer");
            }
            if(It != PeekIterator())
            {
                m_CurrentOffset += ConsumedBytesCount;
                It.m_Offset -= ConsumedBytesCount;
            }
        }

        StreamReader(StreamReader const&) = delete;
        StreamReader(StreamReader&&) = default;
        StreamReader& operator=(StreamReader const&) = delete;
        virtual ~StreamReader()
        {
               
        }

        PeekIterator end()
        {
            PeekIterator ReturnValue;
            ReturnValue.m_Finished = true;
            return ReturnValue;
        }
        //always tries to read atleast one byte
        PeekIterator begin()
        {
            PeekIterator ReturnValue;
            ReturnValue.m_AssociatedReader = this;
            ReturnValue.Increment();
            return ReturnValue;
        }


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
                if(m_CurrentOffset == m_CurrentBufferSize) {
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
        template<typename PredType>
        std::string PeekWhile(PredType const& Pred)
        {
            std::string ReturnValue;
            size_t VirtualOffset = 0;
            while(!EOFReached())
            {
                if(m_CurrentOffset+VirtualOffset == m_CurrentBufferSize)
                {
                    if(m_CurrentOffset+VirtualOffset == m_Buffer.size())
                    {
                        p_ExtendBuffer();
                    }
                    p_AddToBuffer();
                    continue;
                }
                if(Pred(m_Buffer[VirtualOffset+m_CurrentOffset]))
                {
                    ReturnValue += m_Buffer[VirtualOffset+m_CurrentOffset];
                    VirtualOffset += 1;
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
            return MBUtility::Read(*this,Buffer,ReadSize);
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
