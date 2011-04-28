#if !defined(PROPERTY_MEMBER_FUNCTION_VALUE_HPP)
#define PROPERTY_MEMBER_FUNCTION_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyException.h"
#include "PropertyOperators.h"

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, class C, T (C::*g)()>
  class propReadMemberFunction : 
    public virtual IPropertyRead<T>
  {
    public:
    
    explicit propReadMemberFunction(C & c) : 
      cf(c) 
      { 
      }
    
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      if (g == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return((cf.*g)()); 
      }
    
    private:
    
    C & cf;
    
    propReadMemberFunction & operator = (const propReadMemberFunction &);
    
  };

  template <class T, class C, void (C::*s)(T)>
  class propWriteMemberFunction : 
    public virtual IPropertyWrite<T>
  {
    public:
    
    explicit propWriteMemberFunction(C & c) : 
      cf(c) 
      { 
      }
      
    propWriteMemberFunction(C & c,T arg) : 
      cf(c) 
      { 
      if (s == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      (cf.*s)(arg); 
      }
    
    template<class U>
    propWriteMemberFunction(C & c,U arg) : 
      cf(c) 
      { 
      if (s == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      (cf.*s)(static_cast<T>(arg)); 
      }
    
    propWriteMemberFunction & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    propWriteMemberFunction & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (s == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      (cf.*s)(arg); 
      }
    
    private:
    
    C & cf;
    
    propWriteMemberFunction & operator = (const propWriteMemberFunction &);
    
  };

  template <class T, class C, T (C::*g)(), void (C::*s)(T)>
  struct propMemberFunction : 
    propReadMemberFunction<T,C,g> , 
    propWriteMemberFunction<T,C,s>
  {
  
    explicit propMemberFunction(C & c) : 
      propReadMemberFunction<T,C,g>(c) , 
      propWriteMemberFunction<T,C,s>(c) 
      { 
      }
      
    propMemberFunction(const propMemberFunction & arg) : 
      propReadMemberFunction<T,C,g>(arg) , 
      propWriteMemberFunction<T,C,s>(static_cast<const propWriteMemberFunction<T,C,s> &>(arg)) 
      { 
      }
    
    propMemberFunction(C & c,T arg) : 
      propReadMemberFunction<T,C,g>(c) , 
      propWriteMemberFunction<T,C,s>(c,arg) 
      { 
      }
    
    template<class U>
    propMemberFunction(C & c,U arg) : 
      propReadMemberFunction<T,C,g>(c) , 
      propWriteMemberFunction<T,C,s>(c,arg) 
      { 
      }
    
    propMemberFunction & operator = (const propMemberFunction & arg)
      {
      return(*this = static_cast<T>(arg));
      }
    
    propMemberFunction & operator = (T arg) 
      { 
      (static_cast<propWriteMemberFunction<T,C,s> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    propMemberFunction & operator = (U arg) 
      { 
      (static_cast<propWriteMemberFunction<T,C,s> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator ++ (propMemberFunction<T,C,g,s> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator -- (propMemberFunction<T,C,g,s> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator *= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator *= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator /= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator /= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator %= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator %= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator += (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator += (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator -= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator -= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator <<= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator <<= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator >>= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator >>= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator &= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator &= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator ^= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator ^= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T)
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator |= (propMemberFunction<T,C,g,s> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          class C, 
          T (C::*g)(), 
          void (C::*s)(T),
          class U
         > 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propMemberFunction<T,C,g,s> &
  >::type
operator |= (propMemberFunction<T,C,g,s> & first,U second)
  {
  
  typename detail::BinaryROOr<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
}

#if defined(BOOST_MSVC)

#pragma warning (pop)

#endif

#endif
