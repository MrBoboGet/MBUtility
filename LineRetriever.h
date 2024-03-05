#include "MBInterfaces.h"
namespace MBUtility
{
    class LineRetriever
    {
        private:
            MBUtility::MBOctetInputStream* m_InputStream = nullptr;
            std::string m_CurrentLine;
            std::string m_Buffer;
            size_t m_BufferOffset = 0;
            bool m_Finished = false;
            bool m_StreamFinished = false;
            bool m_LineBuffered = false;

            int m_CurrentLineIndex = 0;
        public:
            LineRetriever(MBUtility::MBOctetInputStream* InputStream);

            bool GetLine(std::string& OutLine);
            void DiscardLine();
            std::string& PeekLine();

            int GetLineIndex() const;
            bool Finished();
    };
}
