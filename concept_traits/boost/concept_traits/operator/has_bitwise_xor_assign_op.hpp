///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_BITWISE_XOR_ASSIGN_OP_HPP
#define BOOST_CT_OPERATOR_HAS_BITWISE_XOR_ASSIGN_OP_HPP

#include <boost/mpl/and.hpp>
#include <boost/concept_traits/operator/detail/has_bitwise_assign_binop.hpp>
#include <boost/concept_traits/operator/detail/binop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_bitwise_xor_assign_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT(has_bitwise_xor_assign_op_impl, ^=)

template<class T,class U = T>
struct has_bitwise_xor_assign_op :
  mpl::and_<
    detail::has_bitwise_assign_binop<T,U>,
    detail::has_bitwise_xor_assign_op_impl<T,U>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_BITWISE_XOR_ASSIGN_OP_HPP
