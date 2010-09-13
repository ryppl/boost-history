///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_HAS_SHIFT_OP_HPP
#define BOOST_CT_OPERATOR_DETAIL_HAS_SHIFT_OP_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_integral_or_enum.hpp>
#include <boost/concept_traits/operator/detail/either_is_class_or_union_and_not_void.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// has_shift_op
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct has_shift_op :
  mpl::or_<
    mpl::and_<
      detail::is_integral_or_enum<typename remove_reference<T>::type>,
      detail::is_integral_or_enum<typename remove_reference<U>::type>
    >,
    detail::either_is_class_or_union_and_not_void<T,U>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_HAS_SHIFT_OP_HPP
