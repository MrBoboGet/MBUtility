#include <memory>
#include "TemplateUtilities.h"
#include <variant>
#include <exception>
#include <stdexcept>
#include <utility>


namespace MBUtility
{
       
    template<typename... Types>
    class DynamicVariant
    {
        struct Store
        {
            virtual ~Store()
            {
                   
            }
        };

        template<typename S>
        struct SpecificStore : public Store
        {
            S Value;
            
            template<typename... ConstructArgs>
            SpecificStore(ConstructArgs&&... Args) : Value(std::forward<ConstructArgs>(Args)...) { }
        };
        typedef std::variant<Types...>
            VariantType; 

        std::unique_ptr<Store> m_Content;
        int m_StoredIndex = -1;
    protected:
        template <class T>
        using InVariant = typename std::enable_if<TypeIn_v<std::remove_const_t<std::remove_reference_t<T>>,Types...>>::type;

        template<typename T>
        using enabled_t = std::enable_if<TypeIn_v<T,Types...>>;

        template<typename T>
        static constexpr size_t Index_v = TypeIn_v<std::remove_const_t<std::remove_reference_t<T>>,Types...>;
        
        typedef DynamicVariant<Types...> Base;
    public:   
        DynamicVariant() {
            m_StoredIndex = 0;
            m_Content = std::make_unique<Nth_t<0,Types...>>();
        }

        DynamicVariant(DynamicVariant const&) noexcept = default;
        DynamicVariant(DynamicVariant&&) noexcept = default;
        DynamicVariant& operator=(DynamicVariant&&) noexcept = default;

        template <typename T,typename = InVariant<T>>
        DynamicVariant(T&& Data )
        {
            m_StoredIndex =  Index_v<T>;
            m_Content = std::make_unique<T>(std::forward<T>(Data));
        }
        template <typename T,typename = InVariant<T>>
        DynamicVariant& operator=(T&& Data) noexcept
        {
            m_StoredIndex =  Index_v<T>;
            m_Content = std::make_unique<T>(std::forward<T>(Data));
            return *this;
        }
        template <typename T,typename = InVariant<T>>
        bool IsType() const
        {
            return m_StoredIndex == TypeIndex_v<T,Types...>;
        }
        template <typename T,typename = InVariant<T>>
        T const& GetType() const
        {
            if(!IsType<T>())
            {
                throw std::runtime_error("Invalid access in DynamicVariant");   
            }
            return static_cast<SpecificStore<T> const&>(*m_Content).Value;
        }
        template <typename T,typename = InVariant<T>>
        T& GetType()
        {
            if(!IsType<T>())
            {
                throw std::runtime_error("Invalid access in DynamicVariant");   
            }
            return static_cast<SpecificStore<T>&>(*m_Content).Value;
        }
        template <typename T,typename = InVariant<T>>
        T& GetOrAssign()
        {
            if(!IsType<T>())
            {
                *this = std::make_unique<SpecificStore<T>>();
            }
            return GetType<T>();
        }
        template <typename T,typename... ArgTypes,typename = InVariant<T>>
        T& emplace(ArgTypes&&... Args)
        {
            m_StoredIndex = Index_v<T>;
            m_Content = std::make_unique<SpecificStore<T>>(std::forward<ArgTypes>(Args)...);
            return static_cast<SpecificStore<T>&>(*m_Content).Value;
        }

        ///template <typename T>
        ///void Visit(T& Visitor) const
        ///{
        ///    return std::visit(Visitor,m_Content);
        ///}
    };
};
