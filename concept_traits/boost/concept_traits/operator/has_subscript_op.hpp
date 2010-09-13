///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_SUBSCRIPT_OP_HPP
#define BOOST_CT_OPERATOR_HAS_SUBSCRIPT_OP_HPP

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_integral_or_enum.hpp>
#include <boost/concept_traits/operator/detail/is_object_pointer.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_subscript_op
//
// The subscript operator has to be a member function, so it can only be
// detected for classes and unions by specialising this trait. It gives default
// true for these.
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct has_subscript_op :
  mpl::or_<
    mpl::and_<
      mpl::or_<
        detail::is_object_pointer<typename remove_reference<T>::type>,
        is_array<typename remove_reference<T>::type>
      >,
      detail::is_integral_or_enum<typename remove_reference<U>::type>
    >,
    detail::is_class_or_union_<typename remove_reference<T>::type>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_SUBSCRIPT_OP_HPP
