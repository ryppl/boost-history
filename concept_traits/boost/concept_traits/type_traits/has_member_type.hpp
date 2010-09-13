///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_TT_EXT_HAS_MEMBER_TYPE_HPP
#define BOOST_CT_TT_EXT_HAS_MEMBER_TYPE_HPP

#include <boost/config.hpp> // For BOOST_STATIC_CONSTANT

///////////////////////////////////////////////////////////////////////////////
// BOOST_CT_TT_EXT_DEFINE_HAS_MEMBER_TYPE
///////////////////////////////////////////////////////////////////////////////

#define BOOST_CT_TT_EXT_DEFINE_HAS_MEMBER_TYPE(name)\
/*\
 * g++ 3.2 doesn't work with these as static member functions, so they are put\
 * in a namespace, instead\
 */\
namespace has_member_type_##name##_detail {\
template<class U>\
char test(typename U::name *);\
\
template<class U>\
char (&test(...))[2];\
} /* namespace has_member_type_##name##_detail */\
\
template<class T>\
struct has_member_type_##name\
{\
  typedef has_member_type_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(has_member_type_##name##_detail::test<T>(0))==1);\
};

#endif // BOOST_CT_TT_EXT_HAS_MEMBER_TYPE_HPP
