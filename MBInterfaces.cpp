#include "MBInterfaces.h"

#include <iostream>
namespace MBUtility
{

    //BEGIN TerminalOutput
    size_t TerminalOutput::Write(const void *DataToWrite, size_t DataToWriteSize)
    {
        std::cout.write((const char*) DataToWrite,DataToWriteSize);
        return(DataToWriteSize);
    }
    void TerminalOutput::Flush() 
    {
        std::cout.flush();   
    }
    //END TerminalOutput
}
