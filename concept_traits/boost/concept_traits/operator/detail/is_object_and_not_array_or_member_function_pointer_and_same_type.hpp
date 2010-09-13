///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_OBJECT_AND_NOT_ARRAY_OR_MEMBER_FUNCTION_POINTER_AND_SAME_TYPE_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_OBJECT_AND_NOT_ARRAY_OR_MEMBER_FUNCTION_POINTER_AND_SAME_TYPE_HPP

#include <boost/type_traits/is_array.hpp>
#include <boost/type_traits/is_reference.hpp>
#include <boost/type_traits/is_object.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_member_function_pointer.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/not.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_object_and_not_array_or_member_function_pointer_and_same_type
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct is_object_and_not_array_or_member_function_pointer_and_same_type :
  mpl::and_<
    mpl::or_<
      mpl::and_<
        is_object<typename remove_reference<T>::type>,
        mpl::not_<
          is_array<typename remove_reference<T>::type>
        >
      >,
      is_member_function_pointer<typename remove_reference<T>::type>
    >,
    is_same<
      typename remove_cv<typename remove_reference<T>::type>::type,
      typename remove_cv<typename remove_reference<U>::type>::type
    >
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_OBJECT_AND_NOT_ARRAY_OR_MEMBER_FUNCTION_POINTER_AND_SAME_TYPE_HPP
