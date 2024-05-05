#pragma once

#include <variant>
#include <memory>

namespace MBUtility
{
    template<typename T>
    class SmartPtr
    {
    private:
        T* m_Ptr = nullptr;
        
        std::variant<T*,std::shared_ptr<T>,std::unique_ptr<T>> m_Data;
    public:

        SmartPtr() { }

        SmartPtr(SmartPtr const&) = delete;
        SmartPtr& operator=(SmartPtr const&) = delete;

        friend void swap(SmartPtr& lhs,SmartPtr& rhs)
        {
            std::swap(lhs.m_Ptr,rhs.m_Ptr);
            std::swap(lhs.m_Data,rhs.m_Data);
        }

        SmartPtr(SmartPtr&& rhs) 
        {
            swap(*this,rhs);
        }
        SmartPtr& operator=(SmartPtr&& rhs) 
        {
            swap(*this,rhs);
            return *this;
        }


        SmartPtr(std::shared_ptr<T> Data)
        {
            m_Ptr = Data.get();
            m_Data = std::move(Data);
        }
        SmartPtr(std::unique_ptr<T> Data)
        {
            m_Ptr = Data.get();
            m_Data = std::move(Data);
        }
        explicit SmartPtr(T* Data)
        {
            m_Ptr = Data;
            m_Data = Data;
        }

        friend bool operator==(SmartPtr const& lhs,std::nullptr_t)
        {
            return lhs.m_Ptr == nullptr;
        }
        friend bool operator==(std::nullptr_t,SmartPtr const& rhs)
        {
            return rhs.m_Ptr == nullptr;
        }
        friend bool operator!=(SmartPtr const& lhs,std::nullptr_t)
        {
            return lhs.m_Ptr != nullptr;
        }
        friend bool operator!=(std::nullptr_t,SmartPtr const& rhs)
        {
            return rhs.m_Ptr != nullptr;
        }
        T* get()
        {
            return m_Ptr;
        }
        T const* get() const
        {
            return m_Ptr;
        }
        T& operator*()
        {
            return *m_Ptr;
        }
        T const& operator*() const
        {
            return *m_Ptr;
        }
        T* operator->()
        {
            return m_Ptr;
        }
        T const* operator->() const
        {
            return m_Ptr;
        }
    };
}
