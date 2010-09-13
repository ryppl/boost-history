///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_OBJECT_POINTER_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_OBJECT_POINTER_HPP

#include <boost/type_traits/is_function.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_object_pointer
///////////////////////////////////////////////////////////////////////////////
  
template<class T>
struct is_object_pointer :
  mpl::and_<
    is_pointer<T>,
    mpl::not_<
      is_function<typename remove_pointer<T>::type>
    >
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_OBJECT_POINTER_HPP
