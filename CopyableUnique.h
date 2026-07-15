#include <memory>
namespace MBUtility
{
   
    
    template<typename T>
    class CopyableUnique
    {
        std::unique_ptr<T> m_Data;
    public:
        CopyableUnique() = default;
        CopyableUnique(CopyableUnique&&) = default;
        CopyableUnique& operator=(CopyableUnique&&) = default;

        explicit CopyableUnique(T* Ptr)
            : m_Data(Ptr)
        {
               
        }

        CopyableUnique(CopyableUnique const& Other)
        {
            if(Other.m_Data != nullptr)
            {
                m_Data = std::make_unique<T>(*Other.m_Data);
            }
        }

        CopyableUnique& operator=(CopyableUnique const& Other)
        {
            if(Other.m_Data == nullptr)
            {
                m_Data.reset();
            }
            else
            {
                m_Data = std::make_unique<T>(*Other.m_Data);
            }
        }

        void reset()
        {
            m_Data.reset();   
        }

        bool operator=(std::nullptr_t) const
        {
            return m_Data == nullptr;
        }
        bool operator!=(std::nullptr_t) const
        {
            return m_Data != nullptr;   
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
    template<typename T,typename...Args>
    CopyableUnique<T> MakeCopyableUnique(Args&&... Arguments)
    {
        return CopyableUnique(new T(std::forward<Args>(Arguments)...));
    }
}
