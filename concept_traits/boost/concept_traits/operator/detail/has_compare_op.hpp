///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_HAS_COMPARE_OP_HPP
#define BOOST_CT_OPERATOR_DETAIL_HAS_COMPARE_OP_HPP

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_bounds.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_arithmetic_or_enum.hpp>
#include <boost/concept_traits/operator/detail/either_is_class_or_union_and_not_void.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// has_compare_op
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct has_compare_op :
  mpl::or_<
    mpl::and_<
      detail::is_arithmetic_or_enum<typename remove_reference<T>::type>,
      detail::is_arithmetic_or_enum<typename remove_reference<U>::type>
    >,
    mpl::and_<
      mpl::and_<
        mpl::or_<
          is_array<typename remove_reference<T>::type>,
          is_pointer<typename remove_reference<T>::type>
        >,
        mpl::or_<
          is_array<typename remove_reference<U>::type>,
          is_pointer<typename remove_reference<U>::type>
        >
      >,
      is_same<
        typename remove_cv<typename remove_bounds<typename remove_pointer<typename remove_reference<T>::type>::type>::type>::type,
        typename remove_cv<typename remove_bounds<typename remove_pointer<typename remove_reference<U>::type>::type>::type>::type
      >
    >,
    mpl::and_<
      is_function<typename remove_reference<T>::type>,
      is_same<
        typename remove_cv<typename remove_reference<T>::type>::type,
        typename remove_cv<typename remove_reference<U>::type>::type
      >
    >,
    detail::either_is_class_or_union_and_not_void<T,U>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_HAS_COMPARE_OP_HPP
