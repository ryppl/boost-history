///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_NOT_CONST_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_NOT_CONST_HPP

#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/not.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_not_const
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct is_not_const :
  mpl::not_<
    is_const<typename remove_reference<T>::type>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_NOT_CONST_HPP
