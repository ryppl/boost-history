///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_COMPATIBLE_OBJECT_POINTERS_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_COMPATIBLE_OBJECT_POINTERS_HPP

#include <boost/type_traits/is_base_and_derived.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_object_pointer.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_compatible_object_pointers
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct is_compatible_object_pointers :
  mpl::and_<
    detail::is_object_pointer<typename remove_reference<U>::type>,
    mpl::or_<
      is_same<typename remove_cv<typename remove_reference<T>::type>::type,void *>,
      is_base_and_derived<
        typename remove_cv<typename remove_pointer<typename remove_reference<T>::type>::type>::type,
        typename remove_cv<typename remove_pointer<typename remove_reference<U>::type>::type>::type
      >
    >
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_COMPATIBLE_OBJECT_POINTERS_HPP
