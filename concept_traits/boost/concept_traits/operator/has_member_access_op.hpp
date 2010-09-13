///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_HAS_MEMBER_ACCESS_OP_HPP
#define BOOST_CT_OPERATOR_HAS_MEMBER_ACCESS_OP_HPP

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_class_or_union.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// has_member_access_op
//
// The member access operator has to be a member function, so it can only be
// detected for classes and unions by specialising this trait. It gives default
// true for these.
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct has_member_access_op :
  mpl::or_<
    mpl::and_<
      is_pointer<typename remove_reference<T>::type>,
      detail::is_class_or_union_<typename remove_pointer<typename remove_reference<T>::type>::type>
    >,
    detail::is_class_or_union_<typename remove_reference<T>::type>
  > {};

} // namespace boost

#endif // BOOST_CT_OPERATOR_HAS_MEMBER_ACCESS_OP_HPP
