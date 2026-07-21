#pragma once
#include "TemplateUtilities.h"
#include <tuple>
#include <unordered_map>
#include <typeindex>
#include <memory>

namespace MBUtility
{
    template<typename... T> 
    class Context
    {
        std::tuple<T...> m_StaticElements;
        typedef void(*Deleter)(void*);
        std::unordered_map<std::type_index,std::unique_ptr<void,Deleter>> m_DynamicElements;
    public:
        Context(T&&... Args)
        {
            m_StaticElements = std::make_tuple(std::forward<T>(Args)...);
            m_DynamicElements.reserve(sizeof...(T));
            auto InsertStaticElement = [&]<typename E>(E const& Element)
            {
                m_DynamicElements[std::type_index(typeid(E))] = 
                    std::unique_ptr<void,Deleter>(&std::get<E>(m_StaticElements),[](void*){});
            };
        }

        template<typename U>
        U& Get()
        {
            return const_cast<U&>( ((const Context*)this)->Get<U>());   
        }
        template<typename U>
        U* TryGet()
        {
            return const_cast<U&>( ((const Context*)this)->TryGet<U>());   
        }
        template<typename U>
        U const& Get() const
        {
            auto ReturnValue = TryGet<U>();
            if(ReturnValue == nullptr)
            {
                throw std::runtime_error(std::string("Could not found type with ID ")+typeid(U).name() + " in context");
            }
            return *ReturnValue;
        }
        template<typename U> 
        U const* TryGet() const
        {
            if constexpr (TypeIn_v<U,T...>)
            {
                return &std::get<U>(m_StaticElements);
            }
            else
            {
                auto It = m_DynamicElements.find(std::type_index(typeid(U)));
                if(It == m_DynamicElements.end())
                {
                    return nullptr;
                }
                return reinterpret_cast<U*>(It->second.get());
            }
        }

        template<typename U,typename... A>
        U& Emplace(A&&... Args)
        {
            auto& Result = m_DynamicElements.emplace(std::piecewise_construct, 
                    std::type_index(typeid(U)),
                    std::forward_as_tuple(std::forward<A>(Args)...));
            return Result.second;
        }
    };
}
