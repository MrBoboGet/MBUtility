#pragma once
#include <cstring>
#include <memory>
#include <new>
#include <type_traits>
#include <cstddef>
namespace MBUtility
{
    class EmptyClass
    {
          
    };
    template<typename T,size_t BufferSize>
    class MBPreallocatedBase
    {
    protected:
        alignas(T) char m_PreallocatedBuffer[sizeof(T)*BufferSize];
    };
    template <typename T,size_t BufferSize,typename IndexType = size_t,class Allocator = std::allocator<T>>
    class MBVector : std::conditional<BufferSize != 0,MBPreallocatedBase<T,BufferSize>,EmptyClass>::type
    {
    private:
        static constexpr int GrowthFactor = 2;
        Allocator m_Allocator;
        IndexType m_Capacity = BufferSize;
        IndexType m_Size = 0;
        T* m_DynamicBuffer = nullptr;
        T* p_Data()
        {
            if constexpr( BufferSize != 0)
            {
                if(m_Size <= BufferSize)
                {
                    return std::launder(reinterpret_cast<T*>(MBVector::m_PreallocatedBuffer));
                }
            }
            return(m_DynamicBuffer);
        }
        T const* p_Data() const
        {
            if constexpr( BufferSize != 0)
            {
                if(m_Size <= BufferSize)
                {
                    return std::launder(reinterpret_cast<T const*>(MBVector::m_PreallocatedBuffer));
                }
            }
            return(m_DynamicBuffer);
        }
        void p_Reserve(IndexType NewCapacity)
        {
            if(NewCapacity > m_Capacity)
            {
                IndexType NewBufferSize = m_Capacity*GrowthFactor;
                T* NewBuffer = m_Allocator.allocate(NewBufferSize);
                T* PreviousBuffer = p_Data();
                if constexpr(std:: is_trivially_copyable<T>::value)
                {
                    std::memcpy(NewBuffer,PreviousBuffer,sizeof(T)*m_Size);
                }
                else
                {
                    for(IndexType i = 0; i < m_Size; i++)
                    {
                        new (NewBuffer+i) T(std::move(PreviousBuffer[i]));
                        PreviousBuffer[i].~T();
                    } 
                }
                if(m_Capacity > BufferSize)
                {
                    m_Allocator.deallocate(p_Data(),m_Capacity);
                }
                m_DynamicBuffer = NewBuffer;
                m_Capacity = NewBufferSize;
            }  
        }
        void swap_uninitialized(T& Initialized,T& Uninitialized)
        {
            new (&Uninitialized) T(std::move(Initialized));
            Initialized.~T();
        }
        void swap(MBVector& Left,MBVector& Right) noexcept
        {
            if(std::min(Left.m_Size,Right.m_Size) <= BufferSize)
            {
                //Lesser is guaranteed to use the preallocated buffer
                MBVector* Lesser = Left.m_Size < Right.m_Size ? &Left : &Right;
                MBVector* Greater = Lesser == &Left ? &Right : &Left;
                T* LesserData = Lesser->p_Data(); 
                T* GreaterData = Greater->p_Data(); 
                if(Lesser->m_Size <= BufferSize && Greater->m_Size <= BufferSize)
                {
                    //Both use preallcoated 
                    for(IndexType i = 0; i < std::min(Lesser->m_Size,Greater->m_Size);i++)
                    {
                        std::swap(LesserData[i],GreaterData[i]);
                    }
                    for(IndexType i =  std::min(Lesser->m_Size,Greater->m_Size); i < std::max(Lesser->m_Size,Greater->m_Size);i++)
                    {
                        swap_uninitialized(GreaterData[i],LesserData[i]);
                    }
                }
                else
                {
                    for(IndexType i = 0; i < Lesser->m_Size;i++)
                    {
                        swap_uninitialized(LesserData[i],GreaterData[i]);
                    }
                }
            }
            std::swap(Left.m_DynamicBuffer,Right.m_DynamicBuffer);
            std::swap(Left.m_Allocator,Right.m_Allocator);
            std::swap(Left.m_Capacity,Right.m_Capacity);
            std::swap(Left.m_Size,Right.m_Size);
        }
    public:
        T& operator[](IndexType Index)
        {
            return p_Data()[Index];
        }
        T const& operator[](IndexType Index) const
        {
            return p_Data()[Index];
        }
        void push_back(T ValueToAdd)
        {
            p_Reserve(m_Size+1);
            m_Size += 1;
            T* Buffer = p_Data(); 
            new(Buffer+(m_Size-1)) T(std::move(ValueToAdd));
        }
        size_t size() const
        {
            return(m_Size);
        }
        void reserve(IndexType NewCapacity)
        {
            p_Reserve(NewCapacity);    
        }

        T* data()
        {
            return p_Data();
        }
        T const* data() const
        {
            return p_Data();
        }
        bool operator==(MBVector const& OtherVector) const
        {
            bool ReturnValue = true;
            if(m_Size != OtherVector.m_Size)
            {
                return(false);  
            } 
            if constexpr(std::is_trivially_copyable<T>::value)
            {
                ReturnValue = std::memcmp(p_Data(),OtherVector.p_Data(),sizeof(T)*m_Size) == 0;

            }
            else
            {
                T const* ThisData = p_Data();
                T const* OtherData = OtherVector.p_Data();
                for(IndexType i = 0; i < m_Size;i++)
                {
                    if(ThisData[i] != OtherData[i])
                    {
                        return(false);  
                    } 
                }    
            }
            return(ReturnValue);
        }
        bool operator!=(MBVector const& OtherVector)
        {
            return(!(*this == OtherVector));
        }
        MBVector& operator=(MBVector VectorToCopy)
        {
            swap(*this,VectorToCopy);
            return(*this);
        }
        MBVector() = default;
        MBVector(MBVector&& VectorToSwap) noexcept
        {
            swap(VectorToSwap,*this);       
        }
        MBVector(MBVector const& VectorToCopy)
        {
            p_Reserve(VectorToCopy.m_Size);
            m_Size = VectorToCopy.m_Size;
            T* Data = p_Data();
            if constexpr(std::is_trivially_copyable<T>::value)
            {
                std::memcpy(Data,VectorToCopy.p_Data(),sizeof(T)*VectorToCopy.m_Size);
            }
            else
            { 
                for(IndexType i = 0; i < VectorToCopy.m_Size;i++)
                {
                    new (Data+i) T(VectorToCopy[i]);
                }
            }
        }
        ~MBVector()
        {
            if constexpr(!std::is_trivially_copyable<T>::value)
            {
                return;
            }
            T* Data = p_Data();
            for(IndexType i = 0; i < m_Size;i++)
            {
                Data[i].~T();
            }
            if(m_Size > BufferSize)
            {
                m_Allocator.deallocate(Data,m_Capacity);   
            }
        }
    };
}
