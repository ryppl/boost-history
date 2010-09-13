///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_BITWISE_NOT_OP_HPP
#define BOOST_CT_OPERATOR_HAS_BITWISE_NOT_OP_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_integral_or_enum.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>
#include <boost/concept_traits/operator/detail/unop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_bitwise_not_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP_TRAIT(has_bitwise_not_op_impl, ~)

template<class T>
struct has_bitwise_not_op :
  mpl::and_<
    mpl::or_<
      detail::is_integral_or_enum<typename remove_reference<T>::type>,
      detail::is_class_or_union_<typename remove_reference<T>::type>
    >,
    detail::has_bitwise_not_op_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_BITWISE_NOT_OP_HPP
