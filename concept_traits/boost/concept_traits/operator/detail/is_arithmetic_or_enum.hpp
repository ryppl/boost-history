///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_ARITHMETIC_OR_ENUM_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_ARITHMETIC_OR_ENUM_HPP

#include <boost/type_traits/is_enum.hpp>
#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/mpl/or.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_arithmetic_or_enum
///////////////////////////////////////////////////////////////////////////////
  
template<class T>
struct is_arithmetic_or_enum :
  mpl::or_<
    is_arithmetic<T>,
    is_enum<T>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_ARITHMETIC_OR_ENUM_HPP
