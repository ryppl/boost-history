#if !defined(PROPERTY_CALLABLE_VALUE_HPP)
#define PROPERTY_CALLABLE_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyException.h"
#include "PropertyOperators.h"
#include <boost/function.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        ReadTag,
                        ReadWriteTag
                        >::type
           >
  class propCallable;
  
  template <class T>
  class propCallable<T,ReadTag> : 
    public virtual IPropertyRead<T>
  {

    public:
  
    explicit propCallable(boost::function<T ()> g) : 
      fg(g) 
      { 
      }
  
    operator T() const 
      { 
      return(get()); 
      }
  
    T get() const 
      { 
      if (fg == 0) 
        { 
        throw PropertyReadException(); 
        } 
      return(fg()); 
      }
  
    private:
  
    boost::function<T ()> fg;
    
    propCallable & operator = (const propCallable & arg);
    
  };

  template <class T>
  class propCallable<T,WriteTag> : 
    public virtual IPropertyWrite<T>
  {

    public:
  
    explicit propCallable(boost::function<void (T)> s) : 
      fs(s) 
      { 
      }
      
    propCallable(boost::function<void (T)> s,T arg) : 
      fs(s) 
      { 
      if (fs == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      fs(arg); 
      }
      
    template<class U>
    propCallable(boost::function<void (T)> s,U arg) : 
      fs(s)
      { 
      if (fs == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      fs(static_cast<T>(arg)); 
      }
      
    propCallable & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    propCallable & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (fs == 0) 
        { 
        throw PropertyWriteException(); 
        } 
      fs(arg); 
      }
  
    private:
  
    boost::function<void (T)> fs;
    
    propCallable & operator = (const propCallable & arg);
    
  };
    
  template <class T>
  struct propCallable<T,ReadWriteTag> : 
    propCallable<T,ReadTag>, 
    propCallable<T,WriteTag>,
    IPropertyReadWrite<T>
  {
  
    propCallable(const propCallable & arg) :
      propCallable<T,ReadTag>(arg),
      propCallable<T,WriteTag>(static_cast<const propCallable<T,WriteTag> &>(arg)) 
      {
      }
    
    propCallable(boost::function<T ()> g,boost::function<void (T)> s) : 
      propCallable<T,ReadTag>(g),
      propCallable<T,WriteTag>(s) 
      { 
      }
      
    propCallable(boost::function<T ()> g,boost::function<void (T)> s,T arg) : 
      propCallable<T,ReadTag>(g),
      propCallable<T,WriteTag>(s,arg) 
      { 
      }
      
    template<class U>
    propCallable(boost::function<T ()> g,boost::function<void (T)> s,U arg) : 
      propCallable<T,ReadTag>(g),
      propCallable<T,WriteTag>(s,arg) 
      { 
      }
      
    propCallable & operator = (const propCallable & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    propCallable & operator = (T arg) 
      { 
      (static_cast<propCallable<T,WriteTag> &>(*this)) = arg; 
      return(*this); 
      }
      
    template<class U>
    propCallable & operator = (U arg) 
      { 
      (static_cast<propCallable<T,WriteTag> &>(*this)) = arg; 
      return(*this); 
      }
      
  };

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator ++ (propCallable<T,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator -- (propCallable<T,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator *= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator *= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator /= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator /= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator %= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator %= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator += (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator += (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator -= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator -= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator <<= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator <<= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator >>= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator >>= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator &= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator &= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator ^= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator ^= (propCallable<T,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator |= (propCallable<T,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propCallable<T,ReadWriteTag> &
  >::type
operator |= (propCallable<T,ReadWriteTag> & first,U second)
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
