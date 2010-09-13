///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_LOGICAL_OR_OP_HPP
#define BOOST_CT_OPERATOR_HAS_LOGICAL_OR_OP_HPP

#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/concept_traits/operator/detail/binop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_logical_or_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT(has_logical_or_op_impl, ||)

template<class T,class U = T>
struct has_logical_or_op :
  mpl::and_<
    is_object<typename remove_reference<T>::type>,
    is_object<typename remove_reference<U>::type>,
    detail::has_logical_or_op_impl<T,U>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_LOGICAL_OR_OP_HPP
