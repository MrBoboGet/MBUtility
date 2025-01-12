#pragma once
#include <type_traits>
#include <cmath>




namespace MBUtility
{
    template<typename T>
    inline std::enable_if_t<std::is_integral_v<T>,T> FFS(T Value)
    {
        if(Value == 0)
        {
            return 0;   
        }
        return ((T)std::log2(Value))+1;
    }
}
