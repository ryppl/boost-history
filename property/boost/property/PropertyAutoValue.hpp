#if !defined(PROPERTY_AUTO_VALUE_HPP)
#define PROPERTY_AUTO_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyOperators.h"
#include <boost/utility/value_init.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>
#include <boost/optional.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            template <class> class NotificationPolicy = DefaultPolicyTag,
            class Access = typename boost::mpl::if_
              <
              boost::is_const<T>,
              ReadTag,
              ReadWriteTag
              >::type
           >
  class propAuto;
  
  template <class T>
  class propAuto<T,DefaultPolicyTag,ReadTag> : 
    public virtual IPropertyRead<T>
  {
  
    public:

    propAuto() 
      { 
      }
      
    explicit propAuto(T arg) : 
      data(arg) 
      { 
      }
      
    template<class U>
    explicit propAuto(U arg) :
      data(static_cast<T>(arg))
      {
      }
      
    operator T() const 
      { 
      return(get()); 
      }

    T get() const 
      { 
      return(boost::get(data)); 
      }
    
    protected:

    boost::initialized<T> data;
    
    private:
    
    propAuto & operator = (const propAuto & arg);
      
  };
  
  template <class T>
  struct propAuto<T,DefaultPolicyTag,ReadWriteTag> : 
    propAuto<T,DefaultPolicyTag,ReadTag>, 
    IPropertyReadWrite<T>
  {

    propAuto() 
      { 
      }
      
    propAuto(const propAuto & arg) : 
      propAuto<T,DefaultPolicyTag,ReadTag>(static_cast<const propAuto<T,DefaultPolicyTag,ReadTag> &>(arg))
      { 
      }
      
    explicit propAuto(T arg) : 
      propAuto<T,DefaultPolicyTag,ReadTag>(arg)
      { 
      }
      
    template<class U>
    explicit propAuto(U arg) : 
      propAuto<T,DefaultPolicyTag,ReadTag>(arg)
      {
      }
      
    propAuto & operator = (const propAuto & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
    
    propAuto & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    propAuto & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
      
    void set(T arg) 
      { 
      boost::get(data) = arg; 
      }
    
  };
  
  template <class T,template <class> class NotificationPolicy>
  struct propAuto<T,NotificationPolicy,ReadWriteTag> : 
    propAuto<T,DefaultPolicyTag,ReadTag>, 
    IPropertyReadWrite<T>, 
    NotificationPolicy<T>
  {

    propAuto() 
      { 
      }
      
    propAuto(const propAuto & arg) : 
      propAuto<T,DefaultPolicyTag,ReadTag>(static_cast<const propAuto<T,DefaultPolicyTag,ReadTag> &>(arg))
      { 
      }
      
    explicit propAuto(T arg) : 
      propAuto<T,DefaultPolicyTag,ReadTag>(arg)
      { 
      }
      
    template<class U>
    explicit propAuto(U arg) : 
      propAuto<T,DefaultPolicyTag,ReadTag>(arg)
      {
      }
      
    propAuto & operator = (const propAuto & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
    
    propAuto & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
      
    template<class U>
    propAuto & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      
      T t(*this);
      
      boost::get(data) = arg;
      PropertyChanged(*this,boost::optional<T>(t),arg);
      }
    
  };
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator ++ (propAuto<T,NotificationPolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator -- (propAuto<T,NotificationPolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator *= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator *= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator /= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator /= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator %= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator %= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator += (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator += (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator -= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator -= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator <<= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator <<= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator >>= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator >>= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator &= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator &= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator ^= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator ^= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator |= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,template <class> class NotificationPolicy,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propAuto<T,NotificationPolicy,ReadWriteTag> &
  >::type
operator |= (propAuto<T,NotificationPolicy,ReadWriteTag> & first,U second)
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
