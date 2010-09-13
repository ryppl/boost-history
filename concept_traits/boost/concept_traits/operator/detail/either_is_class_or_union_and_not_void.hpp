///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_EITHER_IS_CLASS_OR_UNION_AND_NOT_VOID_HPP
#define BOOST_CT_OPERATOR_DETAIL_EITHER_IS_CLASS_OR_UNION_AND_NOT_VOID_HPP

#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_not_void.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// either_is_class_or_union_and_not_void
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct either_is_class_or_union_and_not_void :
  mpl::and_<
    mpl::or_<
      is_class_or_union_<typename remove_reference<T>::type>,
      is_class_or_union_<typename remove_reference<U>::type>
    >,
    is_not_void<T>,
    is_not_void<U>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_EITHER_IS_CLASS_OR_UNION_AND_NOT_VOID_HPP
