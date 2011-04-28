#if !defined(PROPERTY_FUNCTOR_VALUE_HPP)
#define PROPERTY_FUNCTOR_VALUE_HPP

#include "PropertyInterface.h"
#include "PropertyTag.h"
#include "PropertyOperators.h"
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

#if defined(BOOST_MSVC)

#pragma warning (push)
#pragma warning (disable: 4250)

#endif

namespace properties
{

  template <class T, 
            class C, 
            class Access = typename boost::mpl::if_
                        <
                        boost::is_const<T>,
                        ReadTag,
                        ReadWriteTag
                        >::type
           >
  class propFunctor;
  
  template <class T, class C>
  class propFunctor<T,C,ReadTag> : 
    public virtual IPropertyRead<T>
  {
  
    public:
    
    explicit propFunctor(C & c) : cf(c) 
      { 
      }
    
    operator T() const 
      { 
      return(get()); 
      }
    
    T get() const 
      { 
      return(cf()); 
      }
    
    private:
    
    C & cf;
    
    propFunctor & operator = (const propFunctor &);
    
  };

  template <class T, class C>
  class propFunctor<T,C,WriteTag> : 
    public virtual IPropertyWrite<T>
  {
  
    public:
    
    explicit propFunctor(C & c) : 
      cf(c) 
      { 
      }
      
    propFunctor(C & c,T arg) : 
      cf(c) 
      { 
      cf(arg); 
      }
    
    template<class U>
    propFunctor(C & c,U arg) : 
      cf(c) 
      { 
      cf(static_cast<T>(arg)); 
      }
    
    propFunctor & operator = (T arg) 
      { 
      set(arg); 
      return(*this); 
      }
    
    template<class U>
    propFunctor & operator = (U arg) 
      { 
      set(static_cast<T>(arg)); 
      return(*this); 
      }
    
    void set(T arg) 
      { 
      cf(arg); 
      }
    
    private:
    
    C & cf;
    
    propFunctor & operator = (const propFunctor &);
    
  };

  template <class T, class C>
  struct propFunctor<T,C,ReadWriteTag> : 
    propFunctor<T,C,ReadTag>, 
    propFunctor<T,C,WriteTag>,
    IPropertyReadWrite<T>
  {
  
    explicit propFunctor(C & c) : 
      propFunctor<T,C,ReadTag>(c), 
      propFunctor<T,C,WriteTag>(c) 
      { 
      }
      
    propFunctor(const propFunctor & arg) : 
      propFunctor<T,C,ReadTag>(arg),
      propFunctor<T,C,WriteTag>(static_cast<const propFunctor<T,C,WriteTag> &>(arg))
      {
      }
    
    propFunctor(C & c,T arg) : 
      propFunctor<T,C,ReadTag>(c) , 
      propFunctor<T,C,WriteTag>(c,arg) 
      { 
      }
    
    template<class U>
    propFunctor(C & c,U arg) : 
      propFunctor<T,C,ReadTag>(c), 
      propFunctor<T,C,WriteTag>(c,arg) 
      { 
      }
    
    propFunctor & operator = (const propFunctor & arg)
      {
      return(*this = static_cast<T>(arg));
      }
      
    propFunctor & operator = (T arg) 
      { 
      (static_cast<propFunctor<T,C,WriteTag> & >(*this)) = arg; 
      return(*this); 
      }
    
    template<class U>
    propFunctor & operator = (U arg) 
      { 
      (static_cast<propFunctor<T,C,WriteTag> & >(*this)) = arg; 
      return(*this); 
      }
    
  };

template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator ++ (propFunctor<T,C,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }
      
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFDecrement<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator -- (propFunctor<T,C,ReadWriteTag> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator *= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator *= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator /= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator /= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator %= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator %= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator += (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator += (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator -= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator -= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator <<= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator <<= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator >>= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator >>= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator &= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator &= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator ^= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator ^= (propFunctor<T,C,ReadWriteTag> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C>
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator |= (propFunctor<T,C,ReadWriteTag> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T, class C, class U>
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  propFunctor<T,C,ReadWriteTag> &
  >::type
operator |= (propFunctor<T,C,ReadWriteTag> & first,U second)
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
