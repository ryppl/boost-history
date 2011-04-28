#pragma once

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyException.h"
#include "PropertyOperators.h"

#pragma warning (push)
#pragma warning (disable: 4250)

namespace properties
{

  template <class T, T (*g)()>
  struct propReadFunction : 
    virtual IPropertyRead<T>
  {
  
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
      return((*g)()); 
      }
      
    private:
    
    propReadFunction & operator = (const propReadFunction &);
    
  };
  
  template <class T, 
            void (*s)(T) 
           >
  struct propWriteFunction : 
    virtual IPropertyWrite<T>
  {
  
    propWriteFunction() 
      { 
      }
      
    explicit propWriteFunction(T arg) 
      { 
      if (s == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      (*s)(arg); 
      }
    
    template<class U>
    explicit propWriteFunction(U arg) 
      { 
      if (s == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      (*s)(static_cast<T>(arg)); 
      }
    
    propWriteFunction & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    propWriteFunction & operator = (U arg) 
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
      (*s)(arg); 
      }
      
    private:
    
    propWriteFunction & operator = (const propWriteFunction &);
    
  };


  template <class T, T (*g)(), void (*s)(T)>
  struct propFunction : 
    propReadFunction<T,g>, 
    propWriteFunction<T,s>
  {
  
    propFunction() 
      { 
      }
      
    explicit propFunction(T arg) : 
      propWriteFunction<T,s>(arg) 
      { 
      }
    
    propFunction(const propFunction & arg) :
      propReadFunction<T,g>(arg),
      propWriteFunction<T,s>(static_cast<const propWriteFunction<T,s> &>(arg))
      { 
      }
      
    template<class U>
    explicit propFunction(U arg) : 
      propWriteFunction<T,s>(arg) 
      { 
      }
    
    propFunction & operator = (const propFunction & arg)
      {
      return(*this = static_cast<T>(arg));
      }
    
    propFunction & operator = (T arg) 
      { 
      (static_cast<propWriteFunction<T,s> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    propFunction & operator = (U arg) 
      { 
      (static_cast<propWriteFunction<T,s> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propFunction<T,g,s> &
  >::type
operator ++ (propFunction<T,g,s> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, 
          T (*g)(),
          void (*s)(T)
          >
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propFunction<T,g,s> &
  >::type
operator -- (propFunction<T,g,s> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propFunction<T,g,s> &
  >::type
operator *= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator *= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator /= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator /= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator %= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator %= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator += (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator += (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator -= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator -= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator <<= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator <<= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator >>= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator >>= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator &= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator &= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator ^= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator ^= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T) 
          >
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propFunction<T,g,s> &
  >::type
operator |= (propFunction<T,g,s> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, 
          T (*g)(),
          void (*s)(T),
          class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propFunction<T,g,s> &
  >::type
operator |= (propFunction<T,g,s> & first,U second)
  {
  
  typename detail::BinaryROOr<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
}

#pragma warning (pop)
