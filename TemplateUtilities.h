#include <type_traits>
namespace MBUtility
{
    template<typename... T>
    struct TypeIndex { };
    template<typename T,typename ... Rest>
    struct TypeIndex<T,T,Rest...>
    {
        size_t value = 0;
    };
    template<typename T,typename Head,typename ... Rest>
    struct TypeIndex<T,Head,Rest...>
    {
        size_t value = TypeIndex<T,Rest...>::value;
    };

    template<typename... T>
    constexpr size_t TypeIndex_v = TypeIndex<T...>::value;



    template<size_t Index,typename... T>
    struct Nth { };
    
    template<typename T,typename... Tail>
    struct Nth<0,T,Tail...> 
    { 
        typedef T type;
    };
    template<size_t Index,typename T,typename... Tail>
    struct Nth<Index,T,Tail...> 
    { 
        typedef typename Nth<Index-1,Tail...>::type type;
    };

    template<size_t Index,typename... Tail>
    using Nth_t = typename Nth<Index,Tail...>::type;

    template<typename... T>
    struct TypeIn  : public std::false_type
    { 
    
    };
    
    template<typename T,typename... Tail>
    struct TypeIn<T,T,Tail...>  : public std::true_type
    { 
    };
    template<typename T,typename Head,typename... Tail>
    struct TypeIn<T,Head,Tail...>  : public TypeIn<T,Tail...>
    { 
    };
    template<typename... Types>
    constexpr bool TypeIn_v = TypeIn<Types...>::value;
}
