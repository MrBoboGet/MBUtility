

namespace MBUtility
{
    template<typename T>
    struct MinMaxImpl
    {
        T const& Object;

        MinMaxImpl(T const& Value)
            : Object(Value)
        {
               
        }
    };

    template<typename T1,typename T2>
    auto const& operator<(MinMaxImpl<T1> lhs,MinMaxImpl<T2> rhs)
    {
        return lhs.Object < rhs.Object ? lhs.Object : rhs.Object;
    }
    template<typename T1,typename T2>
    auto const& operator<(T1 const& lhs,MinMaxImpl<T2> rhs)
    {
        return lhs < rhs.Object ? lhs : rhs.Object;
    }

    template<typename T1,typename T2>
    auto const& operator>(MinMaxImpl<T1> lhs,MinMaxImpl<T2> rhs)
    {
        return lhs.Object > rhs.Object ? lhs.Object : rhs.Object;
    }
    template<typename T1,typename T2>
    auto const& operator>(T1 const& lhs,MinMaxImpl<T2> rhs)
    {
        return lhs > rhs.Object ? lhs : rhs.Object;
    }

    template<typename... ArgTypes>
    auto const& Min(ArgTypes const&... Args)
    {
        return (... < MinMaxImpl(Args));
    }
    template<typename... ArgTypes>
    auto const& Max(ArgTypes const&... Args)
    {
        return (... > MinMaxImpl(Args));
    }
}
