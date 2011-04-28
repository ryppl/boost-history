#pragma once

#include "PropertyTag.h"
#include "PropertyAutoValue.h"
#include "PropertyOperators.h"
#include <boost/function.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_const.hpp>

#pragma warning (push)
#pragma warning (disable: 4250)

namespace properties
{
  
  template <class T, 
            template <class> class NotificationPolicy = DefaultPolicyTag,
            template <class> class PredicateFailurePolicy = DefaultPolicyTag
           >
  struct propAutoPredicate: 
    propAuto<T,NotificationPolicy>,
    PredicateFailurePolicy<T>
  {

    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    explicit propAutoPredicate(boost::function<bool (T)> f) : 
      fs(f) 
      { 
      }
      
    propAutoPredicate(const propAutoPredicate & arg) : 
      propAuto<T,NotificationPolicy>(static_cast<const propAuto<T,NotificationPolicy> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    propAutoPredicate(boost::function<bool (T)> f,T arg) : 
      propAuto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    template<class U>
    propAutoPredicate(boost::function<bool (T)> f,U arg) : 
      propAuto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    propAutoPredicate & operator = (const propAutoPredicate & arg) 
      {
      static_cast<propAuto<T,NotificationPolicy> &>(*this) = static_cast<const propAuto<T,NotificationPolicy> &>(arg); 
      return(*this); 
      }
      
    propAutoPredicate & operator = (T arg) 
      { 
      static_cast<propAuto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    template<class U>
    propAutoPredicate & operator = (U arg) 
      { 
      static_cast<propAuto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propAuto<T,NotificationPolicy>::set(arg);
        }
      else
        {
      
        T t(*this);
      
        PredicateFailure(*this,fs,boost::optional<T>(t),arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
    bool valid(boost::function<bool (T)> f,T arg) const
      {
      return(!f || f(arg));
      }
    
    template<class U>
    bool valid(boost::function<bool (T)> f,U arg) const
      {
      return(!f || f(static_cast<T>(arg)));
      }
      
    T init(boost::function<bool (T)> f,T arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<T> d;
    
        return(boost::get(d)); 
        }
      }
    
    template<class U>
    U init(boost::function<bool (T)> f,U arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<U> d;
    
        return(boost::get(d));
        }
      }
      
  };
  
  template <class T, 
            template <class> class NotificationPolicy
           >
  struct propAutoPredicate<T,NotificationPolicy,DefaultPolicyTag> : 
    propAuto<T,NotificationPolicy>
  {

    BOOST_MPL_ASSERT_NOT((boost::is_const<T>));
    
    explicit propAutoPredicate(boost::function<bool (T)> f) : 
      fs(f) 
      { 
      }
      
    propAutoPredicate(const propAutoPredicate & arg) : 
      propAuto<T,NotificationPolicy>(static_cast<const propAuto<T,NotificationPolicy> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    propAutoPredicate(boost::function<bool (T)> f,T arg) : 
      propAuto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    template<class U>
    propAutoPredicate(boost::function<bool (T)> f,U arg) : 
      propAuto<T,NotificationPolicy>(init(f,arg)),
      fs(f)
      { 
      }
      
    propAutoPredicate & operator = (const propAutoPredicate & arg) 
      {
      static_cast<propAuto<T,NotificationPolicy> &>(*this) = static_cast<const propAuto<T,NotificationPolicy> &>(arg); 
      return(*this); 
      }
      
    propAutoPredicate & operator = (T arg) 
      { 
      static_cast<propAuto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    template<class U>
    propAutoPredicate & operator = (U arg) 
      { 
      static_cast<propAuto<T,NotificationPolicy> &>(*this) = arg; 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propAuto<T,NotificationPolicy>::set(arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
      
    bool valid(boost::function<bool (T)> f,T arg) const
      {
      return(!f || f(arg));
      }
    
    template<class U>
    bool valid(boost::function<bool (T)> f,U arg) const
      {
      return(!f || f(static_cast<T>(arg)));
      }
      
    T init(boost::function<bool (T)> f,T arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<T> d;
    
        return(boost::get(d)); 
        }
      }
    
    template<class U>
    U init(boost::function<bool (T)> f,U arg) const
      {
      if (valid(f,arg))
        {
        return arg;
        }
      else  
        {
        
        boost::initialized<U> d;
    
        return(boost::get(d));
        }
      }
      
  };
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator ++ (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator -- (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator *= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator *= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator /= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator /= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator %= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator %= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator += (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator += (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator -= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator -= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator <<= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator <<= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator >>= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator >>= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator &= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator &= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator ^= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator ^= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator |= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> &
  >::type
operator |= (propAutoPredicate<T,NotificationPolicy,PredicateFailurePolicy> & first,U second)
  {
  
  typename detail::BinaryROOr<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
}

#pragma warning (pop)
