#if !defined(PROPERTY_OPERATORS_ENABLING_HPP)
#define PROPERTY_OPERATORS_ENABLING_HPP

#include "PropertyInterface.h"
#include "PropertyOperatorsDetail.h"
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace properties
{

  namespace detail
  {

  // Arithmetic, pointer, not bool

  template <class T> 
  struct omf_increment
    {
    typedef typename boost::mpl::or_
      <
      boost::is_pointer<T>,
      boost::mpl::and_
        <
        boost::mpl::not_
          <
          boost::is_same<T,bool>
          >,
        boost::is_arithmetic<T>
        >
      > type;
    };
  
  // Arithmetic, pointer, not bool

  template <class T> 
  struct omf_decrement
    {
    typedef typename boost::mpl::or_
      <
      boost::is_pointer<T>,
      boost::mpl::and_
        <
        boost::mpl::not_
          <
          boost::is_same<T,bool>
          >,
        boost::is_arithmetic<T>
        >
      > type;
    };
  
  // Arithmetic, pointer, not bool

  template <class T> 
  struct omf_post_increment
    {
    typedef typename boost::mpl::or_
      <
      boost::is_pointer<T>,
      boost::mpl::and_
        <
        boost::mpl::not_
          <
          boost::is_same<T,bool>
          >,
        boost::is_arithmetic<T>
        >
      > type;
    };
  
  // Arithmetic, pointer, not bool

  template <class T> 
  struct omf_post_decrement
    {
    typedef typename boost::mpl::or_
      <
      boost::is_pointer<T>,
      boost::mpl::and_
        <
        boost::mpl::not_
          <
          boost::is_same<T,bool>
          >,
        boost::is_arithmetic<T>
        >
      > type;
    };
  
  // Arithmetic, enumeration

  template <class T> 
  struct omf_assign_multiply_same 
    {
    typedef typename boost::mpl::or_
      <
      boost::is_arithmetic<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Arithmetic, enumeration

  template <class T,class U> 
  struct omf_assign_multiply
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_arithmetic<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_arithmetic<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  // Arithmetic, enumeration

  template <class T> 
  struct omf_assign_divide_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_arithmetic<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Arithmetic, enumeration

  template <class T,class U> 
  struct omf_assign_divide
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_arithmetic<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_arithmetic<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  // Integral, enumeration

  template <class T> 
  struct omf_assign_modulo_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct omf_assign_modulo
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_integral<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_integral<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  /* Both - arithmetic, enumeration 
      or 
     One - pointer, other integral, enumeration
  */

  template <class T> 
  struct omf_assign_add_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_arithmetic<T>,
      boost::is_enum<T>
      > type;
    };
  
  /* Both - arithmetic, enumeration 
      or 
     One - pointer, other integral, enumeration
  */

  template <class T,class U> 
  struct omf_assign_add
    {
    typedef typename boost::mpl::or_
      <
      boost::mpl::and_
        <
        boost::mpl::or_
          <
          boost::is_arithmetic<T>,
          boost::is_enum<T>
          >,
        boost::mpl::or_
          <
          boost::is_arithmetic<U>,
          boost::is_enum<U>,
          boost::is_convertible<U,T>
          >
        >,
      boost::mpl::and_
        <
        boost::is_pointer<T>,
        boost::mpl::or_
          <
          boost::is_integral<U>,
          boost::is_enum<U>
          >
        >
      > type;
    };
  
  /* Both - arithmetic, enumeration 
      or 
     Both - pointers 
      or
     Left - pointer, other intergral, enumeration
  */

  template <class T> 
  struct omf_assign_subtract_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_arithmetic<T>,
      boost::is_enum<T>,
      boost::is_pointer<T>
      > type;
    };
  
  /* Both - arithmetic, enumeration 
      or 
     Both - pointers 
      or
     Left - pointer, other intergral, enumeration
  */

  template <class T,class U> 
  struct omf_assign_subtract
    {
    typedef typename boost::mpl::or_
      <
      boost::mpl::and_
        <
        boost::mpl::or_
          <
          boost::is_arithmetic<T>,
          boost::is_enum<T>
          >,
        boost::mpl::or_
          <
          boost::is_arithmetic<U>,
          boost::is_enum<U>,
          boost::is_convertible<U,T>
          >
        >,
      boost::mpl::and_
        <
        boost::is_pointer<T>,
        boost::is_pointer<U>,
        boost::is_same
          <
          typename boost::remove_pointer<typename boost::remove_cv<T>::type>::type,
          typename boost::remove_pointer<typename boost::remove_cv<U>::type>::type
          >
        >,
      boost::mpl::and_
        <
        boost::is_pointer<T>,
        boost::mpl::or_
          <
          boost::is_integral<U>,
          boost::is_enum<U>
          >
        >
      > type;
    };
  
  // Integral, enumeration

  template <class T> 
  struct omf_assign_left_shift_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct omf_assign_left_shift
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_integral<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_integral<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  // Integral, enumeration

  template <class T> 
  struct omf_assign_right_shift_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct omf_assign_right_shift
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_integral<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_integral<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  // Integral, enumeration

  template <class T> 
  struct omf_assign_and_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct omf_assign_and
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_integral<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_integral<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  // Integral, enumeration

  template <class T> 
  struct omf_assign_xor_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct omf_assign_xor
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_integral<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_integral<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  
  // Integral, enumeration

  template <class T> 
  struct omf_assign_or_same
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct omf_assign_or
    {
    typedef typename boost::mpl::and_
      <
      boost::mpl::or_
        <
        boost::is_integral<T>,
        boost::is_enum<T>
        >,
      boost::mpl::or_
        <
        boost::is_integral<U>,
        boost::is_enum<U>,
        boost::is_convertible<U,T>
        >
      > type;
    };
  }
  
}

#endif
