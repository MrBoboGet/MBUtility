#pragma once
#include <stdexcept>
#include <type_traits>
namespace MBUtility
{
    template<typename T>
    class Optional
    {
        private:      
            bool m_Initialised = false;
            alignas(T) char m_ObjectData[sizeof(T)];
            
            template<typename... Types>
            void p_Initialize(Types... Args)
            {
                m_Initialised = true;
                new (m_ObjectData) T(Args...);
            }
            T& p_GetRef()
            {
                return(reinterpret_cast<T&>(*m_ObjectData));
            }
            T const& p_GetRef() const
            {
                return(reinterpret_cast<T const&>(*m_ObjectData));
            }
            void p_DeInitilize()
            {
                if(IsInitalized())
                {
                    p_GetRef().~T();
                    m_Initialised = false;
                }
            }
        public:
            Optional()
            {
                
            }
            Optional(T TypeToInitialzie)
            {
                p_Initialize(std::move(TypeToInitialzie)); 
            }
            Optional(Optional const& OptionalToCopy)
            {
                if(OptionalToCopy.IsInitalized())
                {
                    p_Initialize(OptionalToCopy.p_GetRef()); 
                }
            }
            Optional(Optional&& OptionalToCopy)
            {
                if(OptionalToCopy.IsInitalized())
                {
                    p_Initialize(std::move(OptionalToCopy.p_GetRef())); 
                }
            }
            Optional& operator=(T ObjectToInsert)
            {
                if(IsInitalized())
                {
                    p_GetRef() = std::move(ObjectToInsert);
                }
                else
                {
                    p_Initialize(std::move(ObjectToInsert)); 
                }
                return(*this);
            }
            Optional& operator=(Optional ObjectToInsert)
            {
                if(!ObjectToInsert.IsInitalized())
                {
                    if(m_Initialised)
                    {
                        p_DeInitilize();   
                    }
                    return(*this);
                }
                else
                {
                    if(m_Initialised)
                    {
                        p_GetRef() = std::move(ObjectToInsert.p_GetRef());
                    }
                    else
                    {
                        p_Initialize(std::move(ObjectToInsert.p_GetRef()));
                    }
                }
                return(*this);
            }
            operator bool() const
            {
                return m_Initialised;     
            }
            T* operator->()
            {
                if(!IsInitalized())
                {
                    throw std::runtime_error("Trying to access uninitialized optional object");
                }
                return(&p_GetRef());
            }
            T const* operator->() const
            {
                if(!IsInitalized())
                {
                    throw std::runtime_error("Trying to access uninitialized optional object");
                }
                return(&p_GetRef());
            }
            T const& Value() const
            {
                if(!IsInitalized())
                {
                    throw std::runtime_error("Trying to access uninitialized optional object");
                }
                return(p_GetRef());
            }
            T& Value()
            {
                if(!IsInitalized())
                {
                    throw std::runtime_error("Trying to access uninitialized optional object");
                }
                return(p_GetRef());
            }
            bool IsInitalized() const
            {
                return(m_Initialised);   
            }
            ~Optional()
            {
                p_DeInitilize();
            }
    };
}
