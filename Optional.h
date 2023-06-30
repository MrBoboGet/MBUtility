#pragma once
#include <stdexcept>
#include <type_traits>
#include <cstring>
namespace MBUtility
{
    template<typename T>
    class Optional
    {
        private:      
            bool m_Initialised = false;
            alignas(T) char m_ObjectData[sizeof(T)];
            
            template<typename... Types>
            void p_Initialize(Types&&... Args)
            {
                m_Initialised = true;
                new (m_ObjectData) T(std::forward<Types>(Args)...);
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
                if constexpr(!std::is_trivially_destructible<T>::value)
                {
                    if(IsInitalized())
                    {
                        p_GetRef().~T();
                        m_Initialised = false;
                    }
                }
            }
        public:
            Optional()
            {
                
            }
            Optional(T&& TypeToInitialzie) noexcept
            {
                p_Initialize<T&&>(std::move(TypeToInitialzie)); 
            }
            Optional(T const& TypeToInitialzie)
            {
                p_Initialize(TypeToInitialzie); 
            }
            Optional(Optional const& OptionalToCopy)
            {
                if(OptionalToCopy.IsInitalized())
                {
                    p_Initialize(OptionalToCopy.p_GetRef()); 
                }
            }
            Optional(Optional&& OptionalToCopy) noexcept
            {
                if(OptionalToCopy.IsInitalized())
                {
                    p_Initialize<T&&>(std::move(OptionalToCopy.p_GetRef())); 
                }
            }
            Optional& operator=(T const& ObjectToInsert)
            {
                if(IsInitalized())
                {
                    p_GetRef() = ObjectToInsert;
                }
                else
                {
                    p_Initialize(ObjectToInsert); 
                }
                return(*this);
            }
            Optional& operator=(T&& ObjectToInsert) noexcept
            {
                if(IsInitalized())
                {
                    p_GetRef() = ObjectToInsert;
                }
                else
                {
                    p_Initialize<T&&>(std::move(ObjectToInsert));
                }
                return(*this);
            }
            Optional& operator=(Optional const& ObjectToInsert)
            {
                if(!std::is_trivially_copyable_v<T>)
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
                            p_GetRef() = ObjectToInsert.p_GetRef();
                        }
                        else
                        {
                            p_Initialize(ObjectToInsert.p_GetRef());
                        }
                    }
                }
                else
                {
                    if(ObjectToInsert.m_Initialised)
                    {
                        std::memcpy(m_ObjectData,ObjectToInsert.m_ObjectData,sizeof(m_ObjectData));   
                    }
                    m_Initialised = ObjectToInsert.m_Initialised;
                }
                return(*this);
            }
            Optional& operator=(Optional&& ObjectToInsert) noexcept
            {
                if(!std::is_trivially_copyable_v<T>)
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
                            p_Initialize<T&&>(std::move(ObjectToInsert.p_GetRef()));
                        }
                    }
                }
                else
                {
                    if(ObjectToInsert.m_Initialised)
                    {
                        std::memcpy(m_ObjectData,ObjectToInsert.m_ObjectData,sizeof(m_ObjectData));   
                    }
                    m_Initialised = ObjectToInsert.m_Initialised;
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
