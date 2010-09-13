///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_NOT_EQUAL_OP_HPP
#define BOOST_CT_OPERATOR_HAS_NOT_EQUAL_OP_HPP

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_member_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/has_compare_op.hpp>
#include <boost/concept_traits/operator/detail/binop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_not_equal_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT(has_not_equal_op_impl, !=)

template<class T,class U = T>
struct has_not_equal_op :
  mpl::and_<
    mpl::or_<
      detail::has_compare_op<T,U>,
      mpl::and_<
        is_member_pointer<typename remove_reference<T>::type>,
        is_same<
          typename remove_cv<typename remove_reference<T>::type>::type,
          typename remove_cv<typename remove_reference<U>::type>::type
        >
      >
    >,
    detail::has_not_equal_op_impl<T,U>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_NOT_EQUAL_OP_HPP
