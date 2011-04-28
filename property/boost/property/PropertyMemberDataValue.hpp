#if !defined(PROPERTY_MEMBER_DATA_VALUE_HPP)
#define PROPERTY_MEMBER_DATA_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyOperators.h"
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
            class C, 
            T C::*d, 
            template <class> class NotificationPolicy = DefaultPolicyTag,
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        ReadTag,
                        ReadWriteTag
                        >::type
           >
  class propMemberData;
  
  template <class T, class C, T C::*d>
  class propMemberData<T,C,d,DefaultPolicyTag,ReadTag> : 
    public virtual IPropertyRead<T>
  {
    public:
    
    explicit propMemberData(C & c) : 
      cf(c) 
      { 
      }
    
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      return(cf.*d); 
      }
                 
    private:
    
    C & cf;
    
    propMemberData & operator = (const propMemberData &);
  
  };

  template <class T, class C, T C::*d>
  class propMemberData<T,C,d,DefaultPolicyTag,WriteTag> : 
    public virtual IPropertyWrite<T>
  {
    public:
    
    explicit propMemberData(C & c) : 
      cf(c) 
      { 
      }
      
    propMemberData(C & c,T arg) : 
      cf(c)
      { 
      cf.*d = arg;
      }
    
    template<class U>
    propMemberData(C & c,U arg) : 
      cf(c) 
      { 
      cf.*d = static_cast<T>(arg);
      }
    
    propMemberData & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    propMemberData & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      cf.*d = arg; 
      }
    
    private:
    
    C & cf;
  
    propMemberData & operator = (const propMemberData &);
  
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  class propMemberData<T,C,d,NotificationPolicy,WriteTag> : 
    public virtual IPropertyWrite<T>, 
    public NotificationPolicy<T>
  {
    public:
    
    explicit propMemberData(C & c) : 
      cf(c) 
      { 
      }
      
    propMemberData(C & c,T arg) : 
      cf(c) 
      { 
      cf.*d = arg; 
      }
    
    template<class U>
    propMemberData(C & c,U arg) : 
      cf(c) 
      { 
      cf.*d = static_cast<T>(arg); 
      }
    
    propMemberData & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    propMemberData & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      cf.*d = arg; 
      PropertyChanged(*this,oldT,arg);
      }
    
    private:
    
    C & cf;
    
    protected:
    
    boost::optional<T> oldT;
  
    propMemberData & operator = (const propMemberData &);
  
  };

  template <class T, class C, T C::*d>
  struct propMemberData<T,C,d,DefaultPolicyTag,ReadWriteTag> : 
    propMemberData<T,C,d,DefaultPolicyTag,ReadTag>, 
    propMemberData<T,C,d,DefaultPolicyTag,WriteTag>,
    IPropertyReadWrite<T>
  {
    public:
    
    explicit propMemberData(C & c) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(c) , 
      propMemberData<T,C,d,DefaultPolicyTag,WriteTag>(c) 
      { 
      }
      
    propMemberData(const propMemberData & arg) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(arg) , 
      propMemberData<T,C,d,DefaultPolicyTag,WriteTag>(static_cast<const propMemberData<T,C,d,DefaultPolicyTag,WriteTag> &>(arg)) 
      { 
      }
    
    propMemberData(C & c,T arg) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(c) , 
      propMemberData<T,C,d,DefaultPolicyTag,WriteTag>(c,arg) 
      { 
      }
    
    template<class U>
    propMemberData(C & c,U arg) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(c), 
      propMemberData<T,C,d,DefaultPolicyTag,WriteTag>(c,arg) 
      { 
      }
    
    propMemberData & operator = (const propMemberData & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
      
    propMemberData & operator = (T arg) 
      { 
      (static_cast<propMemberData<T,C,d,DefaultPolicyTag,WriteTag> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    propMemberData & operator = (U arg) 
      { 
      (static_cast<propMemberData<T,C,d,DefaultPolicyTag,WriteTag> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

  template <class T, class C, T C::*d,template <class> class NotificationPolicy>
  struct propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> : 
    propMemberData<T,C,d,DefaultPolicyTag,ReadTag>, 
    propMemberData<T,C,d,NotificationPolicy,WriteTag>,
    IPropertyReadWrite<T>
  {
    public:
    
    explicit propMemberData(C & c) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(c), 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c) 
      { 
      }
      
    propMemberData(const propMemberData & arg) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(arg), 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(static_cast<const propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(arg)) 
      { 
      }
    
    propMemberData(C & c,T arg) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(c), 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c,arg)
      { 
      }
    
    template<class U>
    propMemberData(C & c,U arg) : 
      propMemberData<T,C,d,DefaultPolicyTag,ReadTag>(c) , 
      propMemberData<T,C,d,NotificationPolicy,WriteTag>(c,arg) 
      { 
      }
    
    propMemberData & operator = (const propMemberData & arg) 
      {
      return(*this = static_cast<T>(arg));
      }
      
    propMemberData & operator = (T arg) 
      { 
      oldT = *this;
      (static_cast<propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    propMemberData & operator = (U arg) 
      { 
      oldT = *this;
      (static_cast<propMemberData<T,C,d,NotificationPolicy,WriteTag> &>(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator ++ (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator -- (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator *= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator *= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator /= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator /= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator %= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator %= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator += (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator += (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator -= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator -= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator <<= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator <<= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator >>= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator >>= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator &= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator &= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator ^= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator ^= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator |= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, T C::*d,template <class> class NotificationPolicy, class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> &
  >::type
operator |= (propMemberData<T,C,d,NotificationPolicy,ReadWriteTag> & first,U second)
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
