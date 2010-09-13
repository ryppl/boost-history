///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_TT_EXT_HAS_STATIC_MEMBER_HPP
#define BOOST_CT_TT_EXT_HAS_STATIC_MEMBER_HPP

#include <boost/config.hpp> // For BOOST_STATIC_CONSTANT

///////////////////////////////////////////////////////////////////////////////
// BOOST_CT_TT_EXT_DEFINE_HAS_STATIC_MEMBER
///////////////////////////////////////////////////////////////////////////////

#define BOOST_CT_TT_EXT_DEFINE_HAS_STATIC_MEMBER(name)\
template<class T,class Type>\
struct has_static_member_##name\
{\
  template<Type *>\
  struct helper;\
\
  template<class U>\
  static char test(helper<&U::name> *);\
\
  template<class U>\
  static char (&test(...))[2];\
\
  typedef has_static_member_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(test<T>(0))==1);\
};

#endif // BOOST_CT_TT_EXT_HAS_STATIC_MEMBER_HPP
