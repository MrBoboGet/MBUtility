
namespace MBUtility
{
    //Derive from this and implement Increment(), IsEqual and GetRef
    template<typename T,typename IteratedValue>
    class Iterator_Base
    {
    public:
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
        IteratedValue* operator->()
        {
            T& Derived = static_cast<T&>(*this);    
            return(&Derived.GetRef());
        }
        IteratedValue& operator*()
        {
            T& Derived = static_cast<T&>(*this);    
            return(Derived.GetRef());
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
