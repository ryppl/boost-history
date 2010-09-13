///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_TT_EXT_HAS_MEMBER_CLASS_TEMPLATE_HPP
#define BOOST_CT_TT_EXT_HAS_MEMBER_CLASS_TEMPLATE_HPP

#include <boost/config.hpp> // For BOOST_STATIC_CONSTANT
#include <boost/mpl/void.hpp>

///////////////////////////////////////////////////////////////////////////////
// BOOST_CT_TT_EXT_DEFINE_HAS_MEMBER_CLASS_TEMPLATE
///////////////////////////////////////////////////////////////////////////////

#define BOOST_CT_TT_EXT_DEFINE_HAS_MEMBER_CLASS_TEMPLATE(name)\
  BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE5(name)\
  BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE1(name)\
  BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE2(name)\
  BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE3(name)\
  BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE4(name)

#define BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE5(name)\
/*\
 * g++ 3.2 doesn't work with these as static member functions, so they are put\
 * in a namespace, instead\
 */\
namespace has_member_class_template_##name##_detail5 {\
template<\
  class T1,\
  class P1_1,\
  class P2_1,\
  class P3_1,\
  class P4_1,\
  class P5_1\
>\
  char test(typename T1::template name<P1_1,P2_1,P3_1,P4_1,P5_1> *);\
\
template<\
  class T1,\
  class P1_1,\
  class P2_1,\
  class P3_1,\
  class P4_1,\
  class P5_1\
>\
  char (&test(...))[2];\
} /* namespace has_member_class_template_##name##_detail5 */\
\
template<\
  class T,\
  class P1 = ::boost::mpl::void_,\
  class P2 = ::boost::mpl::void_,\
  class P3 = ::boost::mpl::void_,\
  class P4 = ::boost::mpl::void_,\
  class P5 = ::boost::mpl::void_\
>\
struct has_member_class_template_##name\
{\
  typedef has_member_class_template_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(has_member_class_template_##name##_detail5::test<T,P1,P2,P3,P4,P5>(0))==1);\
};

#define BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE1(name)\
/*\
 * g++ 3.2 doesn't work with these as static member functions, so they are put\
 * in a namespace, instead\
 */\
namespace has_member_class_template_##name##_detail1 {\
template<\
  class T1,\
  class P1_1\
>\
char test(typename T1::template name<P1_1> *);\
\
template<\
  class T1,\
  class P1_1\
>\
char (&test(...))[2];\
} /* namespace has_member_class_template_##name##_detail1 */\
\
template<\
  class T,\
  class P1\
>\
struct has_member_class_template_##name<\
  T,\
  P1,\
  ::boost::mpl::void_,\
  ::boost::mpl::void_,\
  ::boost::mpl::void_,\
  ::boost::mpl::void_\
>\
{\
\
  typedef has_member_class_template_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(has_member_class_template_##name##_detail1::test<T,P1>(0))==1);\
};\

#define BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE2(name)\
/*\
 * g++ 3.2 doesn't work with these as static member functions, so they are put\
 * in a namespace, instead\
 */\
namespace has_member_class_template_##name##_detail2 {\
template<\
  class T1,\
  class P1_1,\
  class P2_1\
>\
char test(typename T1::template name<P1_1,P2_1> *);\
\
template<\
  class T1,\
  class P1_1,\
  class P2_1\
>\
char (&test(...))[2];\
} /* namespace has_member_class_template_##name##_detail2 */\
\
template<\
  class T,\
  class P1,\
  class P2\
>\
struct has_member_class_template_##name<\
  T,\
  P1,\
  P2,\
  ::boost::mpl::void_,\
  ::boost::mpl::void_,\
  ::boost::mpl::void_>\
{\
  typedef has_member_class_template_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(has_member_class_template_##name##_detail2::test<T,P1,P2>(0))==1);\
};\

#define BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE3(name)\
/*\
 * g++ 3.2 doesn't work with these as static member functions, so they are put\
 * in a namespace, instead\
 */\
namespace has_member_class_template_##name##_detail3 {\
template<\
  class T1,\
  class P1_1,\
  class P2_1,\
  class P3_1\
>\
char test(typename T1::template name<P1_1,P2_1,P3_1> *);\
\
template<\
  class T1,\
  class P1_1,\
  class P2_1,\
  class P3_1\
>\
char (&test(...))[2];\
} /* namespace has_member_class_template_##name##_detail3 */\
\
template<\
  class T,\
  class P1,\
  class P2,\
  class P3\
>\
struct has_member_class_template_##name<\
  T,\
  P1,\
  P2,\
  P3,\
  ::boost::mpl::void_,\
  ::boost::mpl::void_\
>\
{\
\
  typedef has_member_class_template_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(has_member_class_template_##name##_detail3::test<T,P1,P2,P3>(0))==1);\
};\

#define BOOST_CT_TT_EXT_DETAIL_DEFINE_HAS_MEMBER_CLASS_TEMPLATE4(name)\
/*\
 * g++ 3.2 doesn't work with these as static member functions, so they are put\
 * in a namespace, instead\
 */\
namespace has_member_class_template_##name##_detail4 {\
template<\
  class T1,\
  class P1_1,\
  class P2_1,\
  class P3_1,\
  class P4_1\
>\
char test(typename T1::template name<P1_1,P2_1,P3_1,P4_1> *);\
\
template<\
  class T1,\
  class P1_1,\
  class P2_1,\
  class P3_1,\
  class P4_1\
>\
char (&test(...))[2];\
} /* namespace has_member_class_template_##name##_detail4 */\
\
template<\
  class T,\
  class P1,\
  class P2,\
  class P3,\
  class P4\
>\
struct has_member_class_template_##name<\
  T,\
  P1,\
  P2,\
  P3,\
  P4,\
  ::boost::mpl::void_\
>\
{\
  typedef has_member_class_template_##name type;\
\
  BOOST_STATIC_CONSTANT(bool, value=sizeof(has_member_class_template_##name##_detail4::test<T,P1,P2,P3,P4>(0))==1);\
};

#endif // BOOST_CT_TT_EXT_HAS_MEMBER_CLASS_TEMPLATE_HPP
