///////////////////////////////////////////////////////////////////////////////
// Operator traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_OPERATOR_DETAIL_BINOP_HPP
#define BOOST_CT_OPERATOR_DETAIL_BINOP_HPP

#include <boost/concept_traits/operator/detail/operator_common.hpp>

///////////////////////////////////////////////////////////////////////////////
// BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT
///////////////////////////////////////////////////////////////////////////////

#define BOOST_CT_OPERATOR_DETAIL_DEFINE_BINOP_TRAIT(name,op)\
namespace detail {\
namespace op_test_ns {\
no_match_type operator op(const proxy &,const proxy &);\
\
template<class T,class U>\
struct name\
{\
  static T t;\
  static U u;\
\
  typedef name type;\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(op_test_ns::op_test(t op u))==1);\
};\
} /* namespace op_test_ns */\
template<class T,class U>\
struct name : op_test_ns::name<T,U> {};\
} /* namespace detail */

#endif // BOOST_CT_OPERATOR_DETAIL_BINOP_HPP
