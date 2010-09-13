///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_POST_INCREMENT_OP_HPP
#define BOOST_CT_OPERATOR_HAS_POST_INCREMENT_OP_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>
#include <boost/concept_traits/operator/detail/is_not_const.hpp>
#include <boost/concept_traits/operator/detail/is_object_pointer.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>
#include <boost/concept_traits/operator/detail/unop2.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_post_increment_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP2_TRAIT(has_post_increment_op_impl, ++)

template<class T>
struct has_post_increment_op :
  mpl::and_<
    detail::is_not_const<typename remove_reference<T>::type>,
    mpl::or_<
      is_arithmetic<typename remove_reference<T>::type>,
      detail::is_object_pointer<typename remove_reference<T>::type>,
      detail::is_class_or_union_<typename remove_reference<T>::type>
    >,
    detail::has_pre_increment_op_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_POST_INCREMENT_OP_HPP
