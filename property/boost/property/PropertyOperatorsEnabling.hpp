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
  struct OMFIncrement
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
  struct OMFDecrement
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
  struct OMFPostIncrement
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
  struct OMFPostDecrement
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
  struct OMFAssignMultiplySame 
    {
    typedef typename boost::mpl::or_
      <
      boost::is_arithmetic<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Arithmetic, enumeration

  template <class T,class U> 
  struct OMFAssignMultiply
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
  struct OMFAssignDivideSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_arithmetic<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Arithmetic, enumeration

  template <class T,class U> 
  struct OMFAssignDivide
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
  struct OMFAssignModuloSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct OMFAssignModulo
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
  struct OMFAssignAddSame
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
  struct OMFAssignAdd
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
  struct OMFAssignSubtractSame
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
  struct OMFAssignSubtract
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
  struct OMFAssignLeftShiftSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct OMFAssignLeftShift
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
  struct OMFAssignRightShiftSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct OMFAssignRightShift
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
  struct OMFAssignAndSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct OMFAssignAnd
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
  struct OMFAssignXOrSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct OMFAssignXOr
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
  struct OMFAssignOrSame
    {
    typedef typename boost::mpl::or_
      <
      boost::is_integral<T>,
      boost::is_enum<T>
      > type;
    };
  
  // Integral, enumeration

  template <class T,class U> 
  struct OMFAssignOr
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
