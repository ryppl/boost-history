///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_DIVIDE_OP_HPP
#define BOOST_CT_OPERATOR_HAS_DIVIDE_OP_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_arithmetic_or_enum.hpp>
#include <boost/concept_traits/operator/detail/either_is_class_or_union_and_not_void.hpp>
#include <boost/concept_traits/operator/detail/binop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_divide_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT(has_divide_op_impl, /)

template<class T,class U = T>
struct has_divide_op :
  mpl::and_<
    mpl::or_<
      mpl::and_<
        detail::is_arithmetic_or_enum<typename remove_reference<T>::type>,
        detail::is_arithmetic_or_enum<typename remove_reference<U>::type>
      >,
      detail::either_is_class_or_union_and_not_void<T,U>
    >,
    detail::has_divide_op_impl<T,U>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_DIVIDE_OP_HPP
