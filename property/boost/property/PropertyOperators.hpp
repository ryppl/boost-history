#if !defined(PROPERTY_OPERATORS_HPP)
#define PROPERTY_OPERATORS_HPP

#include "PropertyInterface.h"
#include "PropertyOperatorsEnabling.h"
#include "PropertyOperatorsDetail.h"
#include <boost/utility/enable_if.hpp>

namespace properties
{

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFIncrement<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator ++ (IPropertyReadWrite<T> & value)
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
  IPropertyReadWrite<T> &
  >::type
operator -- (IPropertyReadWrite<T> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFPostIncrement<T>::type,
  T
  >::type
operator ++ (IPropertyReadWrite<T> & value,int)
  {
  
  T ret(value.get());
  T t(ret);
  
  ++t;
  value.set(t);
  return(ret);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFPostDecrement<T>::type,
  T
  >::type
operator -- (IPropertyReadWrite<T> & value,int)
  {
  
  T ret(value.get());
  T t(ret);
  
  --t;
  value.set(t);
  return(ret);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignMultiplySame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator *= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROMultiplySame<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignMultiply<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator *= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROMultiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignDivideSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator /= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryRODivideSame<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignDivide<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator /= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryRODivide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignModuloSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator %= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROModuloSame<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignModulo<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator %= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROModulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignAddSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator += (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROAddSame<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignAdd<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator += (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROAdd<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtractSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator -= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROSubtractSame<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignSubtract<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator -= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROSubtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShiftSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator <<= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROLeftShiftSame<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignLeftShift<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator <<= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROLeftShift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignRightShiftSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator >>= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryRORightShiftSame<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignRightShift<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator >>= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryRORightShift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignAndSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator &= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROAndSame<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::OMFAssignAnd<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator &= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROAnd<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOrSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator ^= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROXOrSame<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignXOr<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator ^= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROXOr<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::OMFAssignOrSame<T>::type,
  IPropertyReadWrite<T> &
  >::type
operator |= (IPropertyReadWrite<T> & first,T second)
  {
  
  typename detail::BinaryROOrSame<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::OMFAssignOr<T,U>::type,
  IPropertyReadWrite<T> &
  >::type
operator |= (IPropertyReadWrite<T> & first,U second)
  {
  
  typename detail::BinaryROOr<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

}

#endif
