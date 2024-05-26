#include <variant>
namespace MBUtility
{
    template<typename... Types>
    class StaticVariant
    {
        typedef std::variant<Types...>
            VariantType; 

        VariantType m_Content;

    protected:
        template <class T>
        using InVariant = typename std::enable_if<std::is_constructible_v<VariantType,T>>::type;
        
        typedef StaticVariant<Types...> Base;
    public:   
        StaticVariant() { }

        template <typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        StaticVariant(T&& Data )
        {
            m_Content = std::forward<T>(Data);
        }
        template <typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        StaticVariant& operator=(T&& Data) noexcept
        {
            m_Content = std::forward<T>(Data);
            return *this;
        }
        template <typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        bool IsType() const
        {
            return std::holds_alternative<T>(m_Content);
        }
        template <typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        T const& GetType() const
        {
            return std::get<T>(m_Content);
        }
        template <typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        T& GetType()
        {
            return std::get<T>(m_Content);
        }
        template <typename T,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        T& GetOrAssign()
        {
            if(!IsType<T>())
            {
                *this = T();
            }
            return GetType<T>();
        }
        template <typename T,typename... ArgTypes,typename = std::enable_if_t<std::is_constructible_v<VariantType,T>>>
        T& emplace(ArgTypes&&... Args)
        {
            m_Content = T(std::forward(Args)...);
            return std::get<T>(m_Content);
        }

        template <typename T>
        auto Visit(T&& Visitor) const
        {
            return std::visit(std::forward<T>(Visitor),m_Content);
        }
        template <typename T>
        auto Visit(T&& Visitor)
        {
            return std::visit(std::forward<T>(Visitor),m_Content);
        }
    };
}
