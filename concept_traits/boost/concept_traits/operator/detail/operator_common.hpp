///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_OPERATOR_COMMON_HPP
#define BOOST_CT_OPERATOR_DETAIL_OPERATOR_COMMON_HPP

namespace boost {
namespace detail {
namespace op_test_ns {

struct proxy
{
  proxy(...);
};

struct no_match_type {};

char op_test(...);
char (&op_test(const no_match_type &))[2];

} // namespace op_test_ns
} // namespace detail
} // namespace boost

#endif // BOOST_CT_OPERATOR_DETAIL_OPERATOR_COMMON_HPP
