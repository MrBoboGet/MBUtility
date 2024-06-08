#pragma once
#include <cstddef>
#include <type_traits>
namespace MBUtility
{
    //Derive from this and implement Increment(), IsEqual and GetRef
    template<typename T,typename ValueType>
    class Iterator_Base
    {
    public:
        typedef std::ptrdiff_t difference_type;
        typedef std::remove_cv_t<ValueType> value_type;
        typedef ValueType* pointer;
        typedef ValueType& reference;
        //T needs to define iterator_category


        T& operator++()
        {
            T& Derived = static_cast<T&>(*this);    
            Derived.Increment();
            return(Derived);
        }
        T  operator++(int)
        {
            T ReturnValue = static_cast<T&>(*this);
            T& Derived = static_cast<T&>(*this);    
            Derived.Increment();
            return(ReturnValue);
        }
        bool operator==(T const& RHS) const
        {
            T const& Derived = static_cast<T const&>(*this);    
            return(Derived.IsEqual(RHS));
        }
        bool operator!=(T const& RHS) const
        {
            T const& Derived = static_cast<T const&>(*this);    
            return(!Derived.IsEqual(RHS));
        }
        auto* operator->()
        {
            T& Derived = static_cast<T&>(*this);    
            return(&Derived.GetRef());
        }
        auto& operator*()
        {
            T& Derived = static_cast<T&>(*this);    
            return(Derived.GetRef());
        }
        auto* operator->() const
        {
            T const& Derived = static_cast<const T&>(*this);    
            return(&Derived.GetRef());
        }
        auto& operator*() const
        {
            T const& Derived = static_cast<const T&>(*this);    
            return(Derived.GetRef());
        }
    };
    //Implements Decrement
    template<typename T,typename ValueType>
    class Bidir_Base : public Iterator_Base<T,ValueType>
    {
    public:
        T& operator--()
        {
            T& Derived = static_cast<T&>(*this);    
            Derived.Decrement();
            return(Derived);
        }
        T  operator--(int)
        {
            T ReturnValue = static_cast<T&>(*this);
            T& Derived = static_cast<T&>(*this);    
            Derived.Decrement();
            return(ReturnValue);
        }
    };
    template<typename T>
    class RangeIterable
    {
    private:
        T m_Begin;
        T m_End;
    public:
        RangeIterable()
        {
               
        }
        RangeIterable(T Begin,T End)
        {
            m_Begin = Begin;
            m_End = End;
        }
        T begin()
        {
            return(m_Begin);
        }
        T end()
        {
            return(m_End);
        }
    };
}
