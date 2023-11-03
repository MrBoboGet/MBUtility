#pragma once
#include "MBInterfaces.h"
#include "IndeterminateInputStream.h"
namespace MBUtility
{
    class SearchableIndeterminateInputStream : public MBSearchableInputStream,public IndeterminateInputStream
    {
    };
}
