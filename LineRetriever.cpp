#include "LineRetriever.h"
namespace MBUtility
{
    //BEGIN LineRetriever
    LineRetriever::LineRetriever(MBUtility::MBOctetInputStream* InputStream)
    {
        m_InputStream = InputStream;       
    }
    bool LineRetriever::Finished()
    {
        return m_Finished;
    }
    int LineRetriever::GetLineIndex() const
    {
        return m_CurrentLineIndex;
    }
    bool LineRetriever::GetLine(std::string& OutLine)
    {
        bool ReturnValue = false;
        if (m_LineBuffered)
        {
            OutLine = m_CurrentLine;
            m_LineBuffered = false;
            ReturnValue = true;
        }
        else if(m_StreamFinished && m_Buffer.size() == 0)
        {
            OutLine = "";  
            m_Finished = true;
            ReturnValue = false;
        } 
        else
        {
            size_t NewlinePosition = m_Buffer.find('\n',m_BufferOffset);
            size_t SearchOffset = m_BufferOffset;
            while(m_StreamFinished == false)
            {
                NewlinePosition = m_Buffer.find('\n',SearchOffset);   
                if(NewlinePosition != m_Buffer.npos)
                {
                    break;   
                }
                SearchOffset = m_Buffer.size();
                constexpr size_t RecieveChunkSize = 4096;
                char RecieveBuffer[RecieveChunkSize];
                size_t RecievedBytes = m_InputStream->Read(RecieveBuffer,RecieveChunkSize);
                m_Buffer += std::string(RecieveBuffer,RecieveBuffer+RecievedBytes);
                if(RecievedBytes < RecieveChunkSize)
                {
                    m_StreamFinished = true;
                    NewlinePosition = m_Buffer.find('\n',SearchOffset);
                }
            }
            if(NewlinePosition == m_Buffer.npos)
            {
                m_CurrentLine = m_Buffer.substr(m_BufferOffset);
                m_Buffer.clear();
                m_BufferOffset = 0;
            }
            else
            {
                if(NewlinePosition > 0 && m_Buffer[NewlinePosition-1] == '\r')
                {
                    m_CurrentLine = m_Buffer.substr(m_BufferOffset,NewlinePosition-1-m_BufferOffset);    
                }
                else
                {
                    m_CurrentLine = m_Buffer.substr(m_BufferOffset,NewlinePosition-m_BufferOffset);    
                }
                m_BufferOffset = NewlinePosition+1;
            }
            if(m_BufferOffset == m_Buffer.size() || m_BufferOffset > 1000000)//a bit arbitrary
            {
                m_Buffer = m_Buffer.substr(m_BufferOffset);
                m_BufferOffset = 0;
            }
            ReturnValue = true;
            OutLine = m_CurrentLine;
            m_LineBuffered = false;
        }
        m_CurrentLineIndex += 1;
        return ReturnValue;
    }
    void LineRetriever::DiscardLine()
    {
        m_LineBuffered = false;
        GetLine(m_CurrentLine); 
        m_LineBuffered = true;
    }
    std::string& LineRetriever::PeekLine()
    {
        if(m_LineBuffered == false)
        {
            GetLine(m_CurrentLine);  
            m_CurrentLineIndex -= 1;
            m_LineBuffered = true;
        } 
        return(m_CurrentLine);
    }
    //END LineRetriever
       
}
