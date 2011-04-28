#pragma once

#include "PropertyTag.h"
#include "PropertyMemberDataValue.h"
#include "PropertyOperators.h"
#include <boost/function.hpp>

#pragma warning (push)
#pragma warning (disable: 4250)

namespace properties
{

  template <class T, 
            class C, 
            T C::*d, 
            template <class> class NotificationPolicy = DefaultPolicyTag,
            template <class> class PredicateFailurePolicy = DefaultPolicyTag,
            class Access = ReadWriteTag
           >
  class propMemberDataPredicate;
  
  template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy>
  struct propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,WriteTag> : 
    propMemberData<T,C,d,NotificationPolicy,WriteTag>,
    PredicateFailurePolicy<T>
  {
    
    propMemberDataPredicate(const propMemberDataPredicate & arg) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(static_cast<const propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(arg)),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,T arg) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,U arg) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    propMemberDataPredicate & operator = (T arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    propMemberDataPredicate & operator = (U arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propMemberData<T,C,d,NotificationPolicy,WriteTag>::set(arg);
        }
      else
        {
        PredicateFailure(*this,fs,boost::optional<T>(),arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
    propMemberDataPredicate & operator = (const propMemberDataPredicate & arg);
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  struct propMemberDataPredicate<T,C,d,NotificationPolicy,DefaultPolicyTag,WriteTag> : 
    propMemberData<T,C,d,NotificationPolicy,WriteTag>
  {
    
    propMemberDataPredicate(const propMemberDataPredicate & arg) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(static_cast<const propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(arg)),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,T arg) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,U arg) : 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    propMemberDataPredicate & operator = (T arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    propMemberDataPredicate & operator = (U arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propMemberData<T,C,d,NotificationPolicy,WriteTag>::set(arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
    propMemberDataPredicate & operator = (const propMemberDataPredicate & arg);
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy>
  struct propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> : 
    propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>,
    PredicateFailurePolicy<T>
  {
    
    propMemberDataPredicate(const propMemberDataPredicate & arg) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(static_cast<const propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(arg)),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(c),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,T arg) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,U arg) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    propMemberDataPredicate & operator = (const propMemberDataPredicate & arg)
      {
      static_cast<propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(*this) = static_cast<const propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(arg);
      return(*this); 
      }
    
    propMemberDataPredicate & operator = (T arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    propMemberDataPredicate & operator = (U arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>::set(arg);
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

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  struct propMemberDataPredicate<T,C,d,NotificationPolicy,DefaultPolicyTag,ReadWriteTag> : 
    propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>
  {
    
    propMemberDataPredicate(const propMemberDataPredicate & arg) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(static_cast<const propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(arg)),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(c),
      fs(f)
      { 
      }
      
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,T arg) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = arg;
        }
      }
    
    template<class U>
    propMemberDataPredicate(boost::function<bool (T)> f,C & c,U arg) : 
      propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>(c),
      fs(f)
      { 
      if (!fs || fs(arg))
        {
        c.*d = static_cast<T>(arg);
        }
      }
    
    propMemberDataPredicate & operator = (const propMemberDataPredicate & arg)
      {
      static_cast<propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(*this) = static_cast<const propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(arg);
      return(*this); 
      }
    
    propMemberDataPredicate & operator = (T arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    template<class U>
    propMemberDataPredicate & operator = (U arg) 
      { 
      static_cast<propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &>(*this) = arg;
      return(*this); 
      }
    
    void set(T arg) 
      { 
      if (!fs || fs(arg))
        {
        propMemberData<T,C,d,NotificationPolicy,ReadWriteTag>::set(arg);
        }
      }
    
    private:
    
    boost::function<bool (T)> fs;
    
  };

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator ++ (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator -- (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator *= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator *= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator /= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator /= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator %= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator %= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator += (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator += (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator -= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator -= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator <<= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator <<= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator >>= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator >>= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator &= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator &= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator ^= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator ^= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator |= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy,template <class> class PredicateFailurePolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> &
  >::type
operator |= (propMemberDataPredicate<T,C,d,NotificationPolicy,PredicateFailurePolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROOr<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
}

#pragma warning (pop)
