///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_COMPATIBLE_POINTERS_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_COMPATIBLE_POINTERS_HPP

#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/concept_traits/operator/detail/is_compatible_object_pointers.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_compatible_pointers
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct is_compatible_pointers :
  mpl::and_<
    is_pointer<typename remove_reference<T>::type>,
    mpl::or_<
      is_same<
        remove_cv<typename remove_reference<T>::type>,
        remove_cv<typename remove_reference<U>::type>
      >,
      detail::is_compatible_object_pointers<T,U>
    >
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_COMPATIBLE_POINTERS_HPP
