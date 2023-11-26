#pragma once

#include "StreamReader.h"
#include "SearchableIndeterminateInputStream.h"
namespace MBUtility
{
    class SearchableStreamReader : public StreamReader,public MBSearchableInputStream
    {
        SearchableIndeterminateInputStream* m_Stream = nullptr;
    public:
        SearchableStreamReader(std::unique_ptr<MBUtility::SearchableIndeterminateInputStream> InputStream)
            : StreamReader(std::unique_ptr<MBUtility::SearchableIndeterminateInputStream>(std::move(InputStream)))
        {
            m_Stream = static_cast<SearchableIndeterminateInputStream*>(p_GetUnderlyingStream());
        }
        uint64_t SetInputPosition(int64_t Offset,int whence = SEEK_SET) 
        {
            m_Stream->SetInputPosition(Offset,whence);
            uint_least64_t CurrentPosition = Position();
            p_ClearBuffer();
            p_ClearEOF();
            if(whence == SEEK_CUR)
            {
                p_SetTotalOffset(CurrentPosition+Offset);
            }
            else if(whence == SEEK_SET)
            {
                p_SetTotalOffset(Offset);
            }
            return Position();
        }
        uint64_t GetInputPosition() 
        {
            return Position();

        }
    };
};
