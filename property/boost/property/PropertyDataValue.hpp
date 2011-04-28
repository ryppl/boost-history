#pragma once

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyException.h"
#include "PropertyOperators.h"
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/optional.hpp>

#pragma warning (push)
#pragma warning (disable: 4250)

namespace properties
{

  template <class T, 
            T *d, 
            template <class> class NotificationPolicy = DefaultPolicyTag,
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        ReadTag,
                        ReadWriteTag
                        >::type
           >
  class propData;
  
  template <class T, T *d>
  struct propData<T,d,DefaultPolicyTag,ReadTag> : 
    virtual IPropertyRead<T>
  {
  
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      return(*d); 
      }
      
    private:  
    
    propData & operator = (const propData &);
    
  };

  template <class T, T *d>
  struct propData<T,d,DefaultPolicyTag,WriteTag> : 
    virtual IPropertyWrite<T>
  {
  
    propData() 
      { 
      }
      
    explicit propData(T arg) 
      { 
      *d = arg; 
      }
      
    template<class U>
    explicit propData(U arg) 
      { 
      *d = static_cast<T>(arg); 
      }
      
    propData & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    propData & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      *d = arg; 
      }
    
    private:  
    
    propData & operator = (const propData &);
    
  };

  template <class T, T *d,template <class> class NotificationPolicy>
  struct propData<T,d,NotificationPolicy,WriteTag> : 
    virtual IPropertyWrite<T>, 
    NotificationPolicy<T>
  {
  
    propData() 
      { 
      }
      
    explicit propData(T arg) 
      { 
      *d = arg; 
      }
      
    template<class U>
    explicit propData(U arg) 
      { 
      *d = static_cast<T>(arg); 
      }
      
    propData & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    propData & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      *d = arg; 
      PropertyChanged(*this,oldT,arg);
      }
      
    protected:
    
    boost::optional<T> oldT;
    
    private:  
    
    propData & operator = (const propData &);
    
  };

  template <class T, T *d>
  struct propData<T,d,DefaultPolicyTag,ReadWriteTag> : 
    propData<T,d,DefaultPolicyTag,ReadTag> , 
    propData<T,d,DefaultPolicyTag,WriteTag>,
    IPropertyReadWrite<T>
  {
  
    propData() 
      { 
      }
      
    propData(const propData & arg) :
      propData<T,d,DefaultPolicyTag,ReadTag>(arg),
      propData<T,d,DefaultPolicyTag,WriteTag>(static_cast<const propData<T,d,DefaultPolicyTag,WriteTag> &>(arg))
      { 
      }
      
    explicit propData(T arg) : 
      propData<T,d,DefaultPolicyTag,WriteTag>(arg) 
      { 
      }
      
    template<class U>
    explicit propData(U arg) : 
      propData<T,d,DefaultPolicyTag,WriteTag>(arg) 
      { 
      }
      
    propData & operator = (const propData & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    propData & operator = (T arg) 
      { 
      (static_cast<propData<T,d,DefaultPolicyTag,WriteTag> & >(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    propData & operator = (U arg) 
      { 
      (static_cast<propData<T,d,DefaultPolicyTag,WriteTag> & >(*this)) = arg; 
      return(*this); 
      }
    
  };

  template <class T, T *d,template <class> class NotificationPolicy>
  struct propData<T,d,NotificationPolicy,ReadWriteTag> : 
    propData<T,d,NotificationPolicy,ReadTag> , 
    propData<T,d,NotificationPolicy,WriteTag>,
    IPropertyReadWrite<T>
  {
  
    propData() 
      { 
      }
      
    propData(const propData & arg) :
      propData<T,d,DefaultPolicyTag,ReadTag>(arg),
      propData<T,d,NotificationPolicy,WriteTag>(static_cast<const propData<T,d,NotificationPolicy,WriteTag> &>(arg))
      { 
      }
      
    explicit propData(T arg) : 
      propData<T,d,NotificationPolicy,WriteTag>(arg) 
      { 
      }
      
    template<class U>
    explicit propData(U arg) : 
      propData<T,d,NotificationPolicy,WriteTag>(arg) 
      { 
      }
      
    propData & operator = (const propData & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    propData & operator = (T arg) 
      { 
      oldT = *this;
      (static_cast<propData<T,d,NotificationPolicy,WriteTag> & >(*this)) = arg; 
      return(*this); 
      }
      
    template<class U>
    propData & operator = (U arg) 
      { 
      oldT = *this;
      (static_cast<propData<T,d,NotificationPolicy,WriteTag> & >(*this)) = arg; 
      return(*this); 
      }
      
  };

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator ++ (propData<T,d,NotificationPolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator -- (propData<T,d,NotificationPolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator *= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator *= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator /= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator /= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator %= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator %= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator += (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator += (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator -= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator -= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator <<= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator <<= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator >>= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator >>= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator &= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator &= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator ^= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator ^= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator |= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, T *d,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propData<T,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator |= (propData<T,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROOr<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
}

#pragma warning (pop)
