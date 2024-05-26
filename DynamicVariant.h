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
            virtual std::unique_ptr<Store> Copy()
            {
                return std::unique_ptr<Store>();
            }
        };

        template<typename S>
        struct SpecificStore : public Store
        {
            S Value;
            
            template<typename... ConstructArgs>
            SpecificStore(ConstructArgs&&... Args) : Value(std::forward<ConstructArgs>(Args)...) { }

            std::unique_ptr<Store> Copy() override
            {
                if constexpr(std::is_copy_constructible_v<S>)
                {
                    return std::make_unique<SpecificStore<S>>(Value);
                }
                else
                {
                    return std::make_unique<Store>();   
                }
            }
        };
        typedef std::variant<Types...>
            VariantType; 

        std::unique_ptr<Store> m_Content;
        int m_StoredIndex = -1;


        template<typename Visitor,typename Head, typename... Type> 
        auto p_Visit(Visitor&& Func)
        {
            if(m_StoredIndex == Index_v<Head>)
            {
                return Func(GetType<Head>());
            }
            else
            {
                if constexpr(sizeof...(Type) == 0)
                {
                    throw std::runtime_error("Internal error visiting DynamicVariant: no appropriate type stored");
                }
                else
                {
                    return p_Visit<Visitor,Type...>(std::forward<Visitor>(Func));
                }
            }
        }
        template<typename Visitor,typename Head, typename... Type> 
        auto p_Visit(Visitor&& Func) const
        {
            if(m_StoredIndex == Index_v<Head>)
            {
                return Func(GetType<Head>());
            }
            else
            {
                if constexpr(sizeof...(Type) == 0)
                {
                    throw std::runtime_error("Internal error visiting DynamicVariant: no appropriate type stored");
                }
                else
                {
                    return p_Visit<Visitor,Type...>(std::forward<Visitor>(Func));
                }
            }
        }
    protected:
        template <class T>
        using InVariant = typename std::enable_if<TypeIn_v<std::remove_const_t<std::remove_reference_t<T>>,Types...>>::type;

        template<typename T>
        using enabled_t = std::enable_if<TypeIn_v<T,Types...>>;

        template<typename T>
        static constexpr size_t Index_v = TypeIndex_v<std::remove_const_t<std::remove_reference_t<T>>,Types...>;
        
    public:   
        DynamicVariant() {
            m_StoredIndex = 0;
            m_Content = std::make_unique<SpecificStore<Nth_t<0,Types...>>>();
        }

        DynamicVariant(DynamicVariant const&) 
        {
            static_assert(std::conjunction_v<std::is_copy_constructible<Types>...>,"DynamicVariant with non-copyable members is not copy-constructible");
        }
        DynamicVariant(DynamicVariant&&) noexcept = default;
        DynamicVariant& operator=(DynamicVariant Rhs) noexcept 
        {
            std::swap(m_Content,Rhs.m_Content);
            std::swap(m_StoredIndex,Rhs.m_StoredIndex);
            return *this;
        }

        template <typename T,typename = InVariant<T>>
        DynamicVariant(T&& Data )
        {
            m_StoredIndex =  Index_v<T>;
            m_Content = std::make_unique<SpecificStore<T>>(std::forward<T>(Data));
        }
        template <typename T,typename = InVariant<T>>
        DynamicVariant& operator=(T&& Data) noexcept
        {
            m_StoredIndex =  Index_v<T>;
            m_Content = std::make_unique<SpecificStore<T>>(std::forward<T>(Data));
            return *this;
        }
        template <typename T,typename = InVariant<T>>
        bool IsType() const
        {
            return m_StoredIndex == Index_v<T>;
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

        template <typename T>
        auto Visit(T&& Visitor) const
        {
            return p_Visit<T,Types...>(std::forward<T>(Visitor));
        }
        template <typename T>
        auto Visit(T&& Visitor) 
        {
            return p_Visit<T,Types...>(std::forward<T>(Visitor));
        }
    };
};
