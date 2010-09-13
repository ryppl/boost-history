///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_UNOP2_HPP
#define BOOST_CT_OPERATOR_DETAIL_UNOP2_HPP

#include <boost/concept_traits/operator/detail/operator_common.hpp>

///////////////////////////////////////////////////////////////////////////////
// BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP2_TRAIT
///////////////////////////////////////////////////////////////////////////////

#define BOOST_CT_OPERATOR_DETAIL_DEFINE_UNOP2_TRAIT(name,op)\
namespace detail {\
namespace op_test_ns {\
no_match_type operator op(const proxy &,int);\
\
template<class T>\
struct name\
{\
  static T t;\
\
  typedef name type;\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(op_test_ns::op_test(t op))==1);\
};\
} /* namespace op_test_ns */\
template<class T>\
struct name : op_test_ns::name<T> {};\
} /* namespace detail */

#endif // BOOST_CT_OPERATOR_DETAIL_UNOP2_HPP
