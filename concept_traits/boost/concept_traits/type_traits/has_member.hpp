///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_TT_EXT_HAS_MEMBER_HPP
#define BOOST_CT_TT_EXT_HAS_MEMBER_HPP

#include <boost/config.hpp>

///////////////////////////////////////////////////////////////////////////////
// BOOST_CT_TT_EXT_DEFINE_HAS_MEMBER
///////////////////////////////////////////////////////////////////////////////

#define BOOST_CT_TT_EXT_DEFINE_HAS_MEMBER(name)\
template<class T>\
struct has_member_##name\
{\
  template<class>\
  struct class_of;\
\
  template<class R,class C>\
  struct class_of<R C::*>\
  {\
    typedef C type;\
  };\
\
  template<T>\
  struct helper;\
\
  template<class U>\
  static char check(helper<&U::name> *);\
\
  template<class U>\
  static char (&check(...))[2];\
\
  typedef has_member_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(check<typename class_of<T>::type>(0))==1);\
};

#endif // BOOST_CT_TT_EXT_HAS_MEMBER_HPP
