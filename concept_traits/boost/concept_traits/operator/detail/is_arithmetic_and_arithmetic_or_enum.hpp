///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_IS_ARITHMETIC_AND_ARITHMETIC_OR_ENUM_HPP
#define BOOST_CT_OPERATOR_DETAIL_IS_ARITHMETIC_AND_ARITHMETIC_OR_ENUM_HPP

#include <boost/type_traits/is_arithmetic.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/mpl/and.hpp>
#include <boost/concept_traits/operator/detail/is_arithmetic_or_enum.hpp>

namespace boost {
namespace detail {

///////////////////////////////////////////////////////////////////////////////
// is_arithmetic_and_arithmetic_or_enum
///////////////////////////////////////////////////////////////////////////////

template<class T,class U>
struct is_arithmetic_and_arithmetic_or_enum :
  mpl::and_<
    is_arithmetic<typename remove_reference<T>::type>,
    detail::is_arithmetic_or_enum<typename remove_reference<U>::type>
  > {};

} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_IS_ARITHMETIC_AND_ARITHMETIC_OR_ENUM_HPP
