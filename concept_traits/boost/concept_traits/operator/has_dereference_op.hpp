///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_DEREFERENCE_OP_HPP
#define BOOST_CT_OPERATOR_HAS_DEREFERENCE_OP_HPP

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_not_void.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>
#include <boost/concept_traits/operator/detail/unop.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_dereference_op
///////////////////////////////////////////////////////////////////////////////

BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP_TRAIT(has_dereference_op_impl, *)

template<class T>
struct has_dereference_op :
  mpl::and_<
    mpl::or_<
      mpl::and_<
        mpl::or_<
          is_pointer<typename remove_reference<T>::type>,
          is_array<typename remove_reference<T>::type>
        >,
        detail::is_not_void<typename remove_pointer<T>::type>
      >,
      detail::is_class_or_union_<typename remove_reference<T>::type>
    >,
    detail::has_dereference_op_impl<T>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_DEREFERENCE_OP_HPP
