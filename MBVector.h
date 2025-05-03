#pragma once
#include <cstring>
#include <memory>
#include <new>
#include <type_traits>
#include <cstddef>

//kinda lmao, used for debugging
#include <vector>
#include <assert.h>
namespace MBUtility
{
    class EmptyClass
    {
          
    };
    class Empty2
    {
          
    };
    template<typename T,size_t BufferSize>
    class MBPreallocatedBase
    {
    protected:
        alignas(T) char m_PreallocatedBuffer[sizeof(T)*BufferSize];
    };

    template<typename T>
    class DebugBase
    {
        protected:
        std::vector<T> DebugVector;
    };
    template <typename T,size_t BufferSize,typename IndexType = size_t,class Allocator = std::allocator<T>,bool Debug=false>
    class MBVector : std::conditional<BufferSize != 0,MBPreallocatedBase<T,BufferSize>,EmptyClass>::type,
                     std::conditional<Debug,DebugBase<T>,Empty2>::type
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
                if(m_Size <= BufferSize && m_DynamicBuffer == nullptr)
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
                if(m_Size <= BufferSize && m_DynamicBuffer == nullptr)
                {
                    return std::launder(reinterpret_cast<T const*>(MBVector::m_PreallocatedBuffer));
                }
            }
            return(m_DynamicBuffer);
        }

        T const* p_GetPreAllocatedBuffer_Const() const
        {
            if constexpr( BufferSize != 0)
            {
                return std::launder(reinterpret_cast<T const*>(MBVector::m_PreallocatedBuffer));
            }
            return m_DynamicBuffer;
        }
        T * p_GetPreAllocatedBuffer() 
        {
            return const_cast<T*>(p_GetPreAllocatedBuffer_Const());
        }
        T const* p_GetPreAllocatedBuffer()  const
        {
            return p_GetPreAllocatedBuffer_Const();
        }
        bool p_IsDynamic() const
        {
            if constexpr( BufferSize != 0)
            {
                if(m_Size <= BufferSize && m_DynamicBuffer == nullptr)
                {
                    return false;
                }
            }
            return true;
        }
        void p_Reserve(IndexType NewCapacity)
        {
            if(NewCapacity > m_Capacity)
            {
                IndexType NewBufferSize = std::max(m_Capacity*GrowthFactor,NewCapacity*2);
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
                //if(m_Capacity > BufferSize && m_DynamicBuffer != nullptr)
                if(p_IsDynamic())
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
            if(!Left.p_IsDynamic() || !Right.p_IsDynamic())
            {
                if(!Left.p_IsDynamic() && !Right.p_IsDynamic())
                {
                    //Lesser is guaranteed to use the preallocated buffer
                    MBVector* Lesser = Left.m_Size < Right.m_Size ? &Left : &Right;
                    MBVector* Greater = Lesser == &Left ? &Right : &Left;
                    T* LesserData = Lesser->p_Data(); 
                    T* GreaterData = Greater->p_Data(); 
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
                    MBVector* NonDynamic = !Left.p_IsDynamic() ? &Left : &Right;
                    MBVector* Dynamic = NonDynamic == &Left ? &Right : &Left;
                    for(IndexType i = 0; i < NonDynamic->m_Size;i++)
                    {
                        swap_uninitialized(NonDynamic->p_GetPreAllocatedBuffer()[i],Dynamic->p_GetPreAllocatedBuffer()[i]);
                    }
                }
            }
            std::swap(Left.m_DynamicBuffer,Right.m_DynamicBuffer);
            std::swap(Left.m_Allocator,Right.m_Allocator);
            std::swap(Left.m_Capacity,Right.m_Capacity);
            std::swap(Left.m_Size,Right.m_Size);
            if constexpr(Debug)
            {
                std::swap(Left.DebugVector,Right.DebugVector);
            }
        }

        void p_AssertDebug()
        {
            if constexpr(Debug)
            {
                assert(*this == MBVector::DebugVector);
            }
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
            if constexpr(Debug)
            {
                MBVector::DebugVector.push_back((*this)[m_Size-1]);
                assert(MBVector::DebugVector == *this);
            }
        }
        template<typename... ArgTypes>
        T& emplace_back(ArgTypes&&... Args)
        {
            p_Reserve(m_Size+1);
            m_Size += 1;
            T* Buffer = p_Data(); 
            new(Buffer+(m_Size-1)) T(std::forward<ArgTypes>(Args)...);
            if constexpr(Debug)
            {
                MBVector::DebugVector.push_back((*this)[m_Size-1]);
                assert(MBVector::DebugVector == *this);
            }
            return back();
        }
        size_t size() const
        {
            return(m_Size);
        }
        T& back()
        {
            if constexpr(Debug)
            {
                assert(p_Data()[m_Size-1] == MBVector::DebugVector.back());
            }
            return p_Data()[m_Size-1];
        }
        T const& back() const
        {
            if constexpr(Debug)
            {
                assert(p_Data()[m_Size-1] == MBVector::DebugVector.back());
            }
            return p_Data()[m_Size-1];
        }

        void resize(size_t NewSize)
        {
            if(NewSize > m_Size)
            {
                for(IndexType i = m_Size; i < NewSize;i++)
                {
                    push_back(T());
                }
            }
            else if(NewSize < m_Size)
            {
                if constexpr(!std::is_trivially_copyable<T>::value)
                {
                    T* Data = p_Data();
                    for(IndexType i = NewSize; i < m_Size;i++)
                    {
                        Data[i].~T();
                    }
                }
            }
            m_Size = NewSize;
            if constexpr(Debug)
            {
                MBVector::DebugVector.resize(NewSize);
                assert(*this == MBVector::DebugVector);
            }
        }
        void pop_back()
        {
            resize(m_Size-1);
        }


        void reserve(IndexType NewCapacity)
        {
            p_Reserve(NewCapacity);    
            if constexpr(Debug)
            {
                assert(*this == MBVector::DebugVector.back());
            }
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
            p_AssertDebug();
            return(*this);
        }
        MBVector() = default;
        MBVector(MBVector&& VectorToSwap) noexcept
        {
            swap(VectorToSwap,*this);       
            p_AssertDebug();
        }
        MBVector(std::initializer_list<T> Elems)
        {
            p_Reserve(Elems.size());
            for(auto const& Value : Elems)
            {
                push_back(Value);   
            }
            p_AssertDebug();
        }

        void clear()
        {
            T* Data = p_Data();
            for(size_t i = 0; i < m_Size;i++)
            {
                Data->~T();   
            }
            m_Size = 0;
        }
        MBVector& operator=(std::initializer_list<T> Elems)
        {
            clear();
            p_Reserve(Elems.size());
            for(auto const& Value : Elems)
            {
                push_back(Value);   
            }
            p_AssertDebug();
        }
        //TODO might have room for optimization
        MBVector(IndexType Count, T const& Value = T())
        {
            p_Reserve(Count);
            m_Size = Count;
            for(auto It = 0; It < Count;It++)
            {
                new(p_Data()+It) T(Value);           
                if constexpr(Debug)
                {
                    MBVector::DebugVector.push_back(Value);
                }
            }
            p_AssertDebug();
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
                    if constexpr(Debug)
                    {
                        MBVector::DebugVector.push_back(VectorToCopy[i]);
                    }
                }
            }
            p_AssertDebug();
        }
        bool operator==(std::vector<T> const& StandardVector) const
        {
            if(size() != StandardVector.size())
            {
                return false;   
            }
            bool ReturnValue = true;
            for(IndexType i = 0; i < size();i++)
            {
                if((*this)[i] != StandardVector[i])
                {
                    return false;   
                }
            }
            return ReturnValue;
        }
        friend bool operator==(std::vector<T> const& lhs,MBVector const& rhs) 
        {
            return rhs == lhs;
        }

        template<size_t BuffSize,typename IndType,typename AllocatorType>
        bool operator<(MBVector<T,BuffSize,IndType,AllocatorType> const& rhs) const
        {
            return std::lexicographical_compare(begin(),end(),rhs.begin(),rhs.end());
        }

        T* begin()
        {
            return p_Data();   
        }
        T* end()
        {
            return p_Data()+m_Size;   
        }
        T const* begin() const
        {
            return p_Data();   
        }
        T const* end() const
        {
            return p_Data()+m_Size;   
        }
        ~MBVector()
        {
            T* Data = p_Data();
            if constexpr(!std::is_trivially_copyable<T>::value)
            {
                for(IndexType i = 0; i < m_Size;i++)
                {
                    Data[i].~T();
                }
            }
            if(p_IsDynamic())
            {
                m_Allocator.deallocate(Data,m_Capacity);   
            }
        }
    };
}
