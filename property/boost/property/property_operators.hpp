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
  typename detail::omf_increment<T>::type,
  i_property_read_write<T> &
  >::type
operator ++ (i_property_read_write<T> & value)
  {
  
  T t(value.get());
  
  ++t;
  value.set(t);
  return(value);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_decrement<T>::type,
  i_property_read_write<T> &
  >::type
operator -- (i_property_read_write<T> & value)
  {
  
  T t(value.get());
  
  --t;
  value.set(t);
  return(value);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_post_increment<T>::type,
  T
  >::type
operator ++ (i_property_read_write<T> & value,int)
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
  typename detail::omf_post_decrement<T>::type,
  T
  >::type
operator -- (i_property_read_write<T> & value,int)
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
  typename detail::omf_assign_multiply_same<T>::type,
  i_property_read_write<T> &
  >::type
operator *= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_multiply_same<T>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_multiply<T,U>::type,
  i_property_read_write<T> &
  >::type
operator *= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_multiply<T,U>::type t(first.get() * second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_divide_same<T>::type,
  i_property_read_write<T> &
  >::type
operator /= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_divide_same<T>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_divide<T,U>::type,
  i_property_read_write<T> &
  >::type
operator /= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_divide<T,U>::type t(first.get() / second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo_same<T>::type,
  i_property_read_write<T> &
  >::type
operator %= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_modulo_same<T>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_modulo<T,U>::type,
  i_property_read_write<T> &
  >::type
operator %= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_modulo<T,U>::type t(first.get() % second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_add_same<T>::type,
  i_property_read_write<T> &
  >::type
operator += (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_add_same<T>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_add<T,U>::type,
  i_property_read_write<T> &
  >::type
operator += (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_add<T,U>::type t(first.get() + second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract_same<T>::type,
  i_property_read_write<T> &
  >::type
operator -= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_subtract_same<T>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_subtract<T,U>::type,
  i_property_read_write<T> &
  >::type
operator -= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_subtract<T,U>::type t(first.get() - second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift_same<T>::type,
  i_property_read_write<T> &
  >::type
operator <<= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_left_shift_same<T>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_left_shift<T,U>::type,
  i_property_read_write<T> &
  >::type
operator <<= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_left_shift<T,U>::type t(first.get() << second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift_same<T>::type,
  i_property_read_write<T> &
  >::type
operator >>= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_right_shift_same<T>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_right_shift<T,U>::type,
  i_property_read_write<T> &
  >::type
operator >>= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_right_shift<T,U>::type t(first.get() >> second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_and_same<T>::type,
  i_property_read_write<T> &
  >::type
operator &= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_and_same<T>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U>
typename boost::enable_if
  <
  typename detail::omf_assign_and<T,U>::type,
  i_property_read_write<T> &
  >::type
operator &= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_and<T,U>::type t(first.get() & second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor_same<T>::type,
  i_property_read_write<T> &
  >::type
operator ^= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_xor_same<T>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_xor<T,U>::type,
  i_property_read_write<T> &
  >::type
operator ^= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_xor<T,U>::type t(first.get() ^ second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T> 
typename boost::enable_if
  <
  typename detail::omf_assign_or_same<T>::type,
  i_property_read_write<T> &
  >::type
operator |= (i_property_read_write<T> & first,T second)
  {
  
  typename detail::binary_ro_or_same<T>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }
  
template <class T,class U> 
typename boost::enable_if
  <
  typename detail::omf_assign_or<T,U>::type,
  i_property_read_write<T> &
  >::type
operator |= (i_property_read_write<T> & first,U second)
  {
  
  typename detail::binary_ro_or<T,U>::type t(first.get() | second);
  
  first.set(static_cast<T>(t));
  return(first);
  }

}

#endif
