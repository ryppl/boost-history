///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_UNARY_MINUS_OP_HPP
#define BOOST_CT_OPERATOR_HAS_UNARY_MINUS_OP_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/unop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_unary_minus_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP_TRAIT(has_unary_minus_op_impl, -)

template<class T>
struct has_unary_minus_op :
  mpl::and_<
    mpl::or_<
      detail::is_arithmetic_or_enum<typename remove_reference<T>::type>,
      detail::is_class_or_union_<typename remove_reference<T>::type>
    >,
    detail::has_unary_minus_op_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_UNARY_MINUS_OP_HPP
