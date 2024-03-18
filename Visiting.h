#include <type_traits>
#include <utility>

namespace MBUtility
{
    template<typename... VisitorClasses> 
    class VisitorCombiner
    {
    public:
        VisitorCombiner()
        {
               
        }
        template<typename... T>
        void operator()(T&&... Args)
        {
            static_assert(sizeof...(Args) != sizeof...(Args) ,"VisitorCombiner called with arguments that no case handles");
        }
        template<typename T>
        struct CaseIsHandled : std::false_type{};
    };
    template<typename Head,typename... Tail>
    class VisitorCombiner<Head,Tail...> : VisitorCombiner<Tail...>
    {
        Head m_Data;
        typedef VisitorCombiner<Tail...> Base;

        
    public:
        template<typename... Types>
        struct CaseIsHandled : std::conditional_t<std::is_invocable_v<Head,Types...>,std::true_type,typename Base::template CaseIsHandled<Types...>>  { };

        VisitorCombiner(Head&& HeadData,Tail&&... TailData) : VisitorCombiner<Tail...>(std::forward<Tail>(TailData)...),
                                                              m_Data(std::forward<Head>(HeadData))
        {

        }


        template<typename... ArgTypes, typename = std::enable_if_t<CaseIsHandled<ArgTypes...>::value>>
        //template<typename... ArgTypes>
        auto operator()(ArgTypes&&... Args)
        {
            if constexpr(std::is_invocable_v<Head,ArgTypes...>)
            {
                return m_Data(std::forward<ArgTypes>(Args)...);
            }
            else
            {
                return static_cast<Base>(*this)(std::forward<ArgTypes>(Args)...);
            }
        }
    };

    template<typename... Args>
    VisitorCombiner(Args&&...) -> VisitorCombiner< std::remove_cv_t<std::remove_reference_t<Args>>...>;
}
