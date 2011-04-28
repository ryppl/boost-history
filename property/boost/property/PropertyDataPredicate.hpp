#if !defined(PROPERTY_DATA_PREDICATE_HPP)
#define PROPERTY_DATA_PREDICATE_HPP

#include "PropertyTag.h"
#include "PropertyDataValue.h"
#include "PropertyOperators.h"
#include <boost/function.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{
  template <class T, 
            T *d, 
            template <class> class NotificationPolicy = DefaultPolicyTag,
            template <class> class PredicateFailurePolicy = DefaultPolicyTag,
            class Access = ReadWriteTag
           >
  class propDataPredicate;
  
  template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy>
  struct propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,WriteTag> : 
    propData<T,d,NotificationPolicy,WriteTag>,
    PredicateFailurePolicy<T>
  {
  
    explicit propDataPredicate(boost::function<bool (T)> f) : 
      fs(f)
      { 
      }
      
    propDataPredicate(const propDataPredicate & arg) : 
      propData<T,d,NotificationPolicy,WriteTag>(static_cast<const propData<T,d,NotificationPolicy,WriteTag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    propDataPredicate(boost::function<bool (T)> f,T arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = arg;
        }
      }
      
    template<class U>
    propDataPredicate(boost::function<bool (T)> f,U arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = static_cast<T>(arg);
        }
      }
      
    propDataPredicate & operator = (T arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    template<class U>
    propDataPredicate & operator = (U arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propData<T,d,NotificationPolicy,WriteTag>::set(arg);
        }
      else
        {
        PredicateFailure(*this,fs,boost::optional<T>(),arg);
        }
      }
      
    private:
    
    boost::function<bool (T)> fs;
      
    propDataPredicate & operator = (const propDataPredicate & arg);
      
  };
  
  template <class T, T *d,template <class> class NotificationPolicy>
  struct propDataPredicate<T,d,NotificationPolicy,DefaultPolicyTag,WriteTag> : 
    propData<T,d,NotificationPolicy,WriteTag>
  {
  
    explicit propDataPredicate(boost::function<bool (T)> f) : 
      fs(f)
      { 
      }
      
    propDataPredicate(const propDataPredicate & arg) : 
      propData<T,d,NotificationPolicy,WriteTag>(static_cast<const propData<T,d,NotificationPolicy,WriteTag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    propDataPredicate(boost::function<bool (T)> f,T arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = arg;
        }
      }
      
    template<class U>
    propDataPredicate(boost::function<bool (T)> f,U arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = static_cast<T>(arg);
        }
      }
      
    propDataPredicate & operator = (T arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    template<class U>
    propDataPredicate & operator = (U arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propData<T,d,NotificationPolicy,WriteTag>::set(arg);
        }
      }
      
    private:
    
    boost::function<bool (T)> fs;
      
    propDataPredicate & operator = (const propDataPredicate & arg);
      
  };
  
  template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy>
  struct propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> : 
    propData<T,d,NotificationPolicy,ReadWriteTag>,
    PredicateFailurePolicy<T>
  {
  
    explicit propDataPredicate(boost::function<bool (T)> f) : 
      fs(f)
      { 
      }
      
    propDataPredicate(const propDataPredicate & arg) : 
      propData<T,d,NotificationPolicy,ReadWriteTag>(static_cast<const propData<T,d,NotificationPolicy,ReadWriteTag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    propDataPredicate(boost::function<bool (T)> f,T arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = arg;
        }
      }
      
    template<class U>
    propDataPredicate(boost::function<bool (T)> f,U arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = static_cast<T>(arg);
        }
      }
      
    propDataPredicate & operator = (const propDataPredicate & arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,ReadWriteTag> &>(*this) = static_cast<const propData<T,d,NotificationPolicy,ReadWriteTag> &>(arg);
      return(*this); 
      }
      
    propDataPredicate & operator = (T arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    template<class U>
    propDataPredicate & operator = (U arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propData<T,d,NotificationPolicy,ReadWriteTag>::set(arg);
        }
      else
        {
      
        T t(*this);
      
        PredicateFailure(*this,fs,boost::optional<T>(t),arg);
        }
      }
      
    private:
    
    boost::function<bool (T)> fs;
      
  };

  template <class T, T *d,template <class> class NotificationPolicy>
  struct propDataPredicate<T,d,NotificationPolicy,DefaultPolicyTag,ReadWriteTag> : 
    propData<T,d,NotificationPolicy,ReadWriteTag>
  {
  
    explicit propDataPredicate(boost::function<bool (T)> f) : 
      fs(f)
      { 
      }
      
    propDataPredicate(const propDataPredicate & arg) : 
      propData<T,d,NotificationPolicy,ReadWriteTag>(static_cast<const propData<T,d,NotificationPolicy,ReadWriteTag> &>(arg)),
      fs(arg.fs)
      { 
      }
      
    propDataPredicate(boost::function<bool (T)> f,T arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = arg;
        }
      }
      
    template<class U>
    propDataPredicate(boost::function<bool (T)> f,U arg) : 
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        *d = static_cast<T>(arg);
        }
      }
      
    propDataPredicate & operator = (const propDataPredicate & arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,ReadWriteTag> &>(*this) = static_cast<const propData<T,d,NotificationPolicy,ReadWriteTag> &>(arg);
      return(*this); 
      }
      
    propDataPredicate & operator = (T arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    template<class U>
    propDataPredicate & operator = (U arg) 
      { 
      static_cast<propData<T,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
      
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propData<T,d,NotificationPolicy,ReadWriteTag>::set(arg);
        }
      }
      
    private:
    
    boost::function<bool (T)> fs;
      
  };

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator ++ (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator -- (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator *= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator *= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator /= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator /= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator %= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator %= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator += (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator += (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator -= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator -= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator <<= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator <<= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator >>= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator >>= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator &= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator &= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator ^= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator ^= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator |= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator |= (propDataPredicate<T,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
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
