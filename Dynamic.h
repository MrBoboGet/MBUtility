#include <memory>
namespace MBUtility
{
    template<typename T>
    class Dynamic
    {
        std::unique_ptr<T> m_Data;
    public:

        
        template<typename... Args>
        Dynamic(Args&&... Arguments)
        {
            m_Data = std::make_unique<T>(std::forward<Args>(Arguments)...);
        }
        Dynamic(T const& Argument)
        {
            m_Data = std::make_unique<T>(Argument);
        }
        Dynamic(T&& Argument) noexcept
        {
            m_Data = std::make_unique<T>(std::move(Argument));
        }
        Dynamic(Dynamic const& Argument)
        {
            m_Data = std::make_unique<T>(*Argument.m_Data);
        }
        Dynamic(Dynamic&& Argument) noexcept
        {
            std::swap(m_Data,Argument.m_Data);
        }


        template<typename Arg>
        T& operator=(Arg&& Argument)
        {
            *m_Data = std::forward<Arg>(Argument);
            return *m_Data;
        }
        Dynamic& operator=(T ValueToCopy)
        {
            std::swap(*m_Data,ValueToCopy);
            return *this;
        }
        Dynamic& operator=(Dynamic ValueToCopy)
        {
            std::swap(m_Data,ValueToCopy.m_Data);
            return *this;
        }
        T* operator->()
        {
            return m_Data.get();
        }
        T const* operator->() const
        {
            return m_Data.get();
        }
        T& operator*()
        {
            return *m_Data;
        }
        T const& operator*() const
        {
            return *m_Data;
        }
    };
}
