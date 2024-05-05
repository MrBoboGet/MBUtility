#pragma once
#include <utility>
#include <memory>
#include <stdexcept>


namespace MBUtility
{
    template<typename ReturnType,typename... ArgTypes>
    class i_FunctionInterface
    {
        public:
        virtual ReturnType Invoke(ArgTypes... Args) = 0;
    };

    template<typename StoredType,typename ReturnType,typename... ArgTypes>
    class i_FunctionStorer : public i_FunctionInterface<ReturnType,ArgTypes...>
    {
        StoredType m_StoredValue;
        public:
        virtual ReturnType Invoke(ArgTypes... Args) override
        {
            return(m_StoredValue(Args...));
        }
        i_FunctionStorer(StoredType&& ValueToStore) : m_StoredValue(std::move(ValueToStore))
        {
        }
        i_FunctionStorer(i_FunctionStorer const& Copy) = delete;
        i_FunctionStorer& operator=(i_FunctionStorer const& ValueToStore) = delete;
    };
    
    template<typename ReturnType,typename... ArgTypes>
    class MOFunction;
    template<typename ReturnType,typename... ArgTypes>
    class MOFunction<ReturnType(ArgTypes...)>
    {
        std::unique_ptr<i_FunctionInterface<ReturnType,ArgTypes...>> m_StoredFunction = nullptr;
    public:
        MOFunction(MOFunction const&) = delete;
        MOFunction(MOFunction&& FunctionToSwap) noexcept 
        {
            std::swap(m_StoredFunction,FunctionToSwap.m_StoredFunction);
        }
        MOFunction& operator=(MOFunction const& ) = delete;
        MOFunction& operator=(MOFunction&& FunctionToMove) = default;
        MOFunction()
        {

        }
        bool IsEmpty() const
        {
            return m_StoredFunction == nullptr;
        }
        ReturnType operator()(ArgTypes... Args)
        {
            if(m_StoredFunction == nullptr)
            {
                throw std::runtime_error("Cannot invoke empty function");     
            }
            return(m_StoredFunction->Invoke(Args...));
        }
        template<typename StoredType>
        MOFunction(StoredType&& FunctionToStore)
        {
            //m_StoredFunction = std::make_unique<i_FunctionStorer<StoredType,ReturnType,ArgTypes...>>(std::move(FunctionToStore));
            m_StoredFunction = std::unique_ptr<i_FunctionStorer<StoredType,ReturnType,ArgTypes...>>( 
                    new i_FunctionStorer<StoredType,ReturnType,ArgTypes...>(std::move(FunctionToStore)));
        }

        friend bool operator==(MOFunction const& lhs, std::nullptr_t)
        {
            return lhs.m_StoredFunction == nullptr;   
        }
        friend bool operator==(std::nullptr_t,MOFunction const& rhs)
        {
            return rhs.m_StoredFunction == nullptr;   
        }
        friend bool operator!=(std::nullptr_t,MOFunction const& rhs)
        {
            return !(rhs == nullptr);
        }
        friend bool operator!=(MOFunction const& lhs,std::nullptr_t)
        {
            return !(lhs == nullptr);
        }
    };
};
