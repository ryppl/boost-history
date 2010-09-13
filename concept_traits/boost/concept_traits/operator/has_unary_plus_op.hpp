///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_UNARY_PLUS_OP_HPP
#define BOOST_CT_OPERATOR_HAS_UNARY_PLUS_OP_HPP

#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/concept_traits/operator/detail/unop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_unary_plus_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP_TRAIT(has_unary_plus_op_impl, +)

template<class T>
struct has_unary_plus_op :
  mpl::and_<
    is_object<typename remove_reference<T>::type>,
    mpl::not_<
      is_member_pointer<typename remove_reference<T>::type>
    >,
    detail::has_unary_plus_op_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_UNARY_PLUS_OP_HPP
