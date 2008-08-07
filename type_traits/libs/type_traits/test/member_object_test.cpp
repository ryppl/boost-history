
//  (C) Copyright John Maddock 2008. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.tt.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_type.hpp"
#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/member_object.hpp>
#endif

struct t{};

BOOST_DECL_TRANSFORM_TEST(member_object_test_1, ::tt::member_object, const, const)
BOOST_DECL_TRANSFORM_TEST(member_object_test_2, ::tt::member_object, volatile, volatile)
BOOST_DECL_TRANSFORM_TEST0(member_object_test_4, ::tt::member_object)
BOOST_DECL_TRANSFORM_TEST(member_object_test_5, ::tt::member_object, const &, const&)
BOOST_DECL_TRANSFORM_TEST(member_object_test_6, ::tt::member_object, &, &)
BOOST_DECL_TRANSFORM_TEST(member_object_test_8, ::tt::member_object, const [2], const[2])
BOOST_DECL_TRANSFORM_TEST(member_object_test_9, ::tt::member_object, const &, const&)
BOOST_DECL_TRANSFORM_TEST(member_object_test_12, ::tt::member_object, const[2][3], const[2][3])
BOOST_DECL_TRANSFORM_TEST(member_object_test_13, ::tt::member_object, (&)[2], (&)[2])

void member_object_test_14()
{
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*const)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*const volatile)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*volatile)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<const int (t::*)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<const int (t::*const)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<const int (t::*const volatile)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<const int (t::*volatile)>::type);

   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*)(short)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*const)(short)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*const volatile)(short)>::type);
   BOOST_CHECK_TYPE(t, ::tt::member_object<int (t::*volatile)(short)>::type);

   BOOST_CHECK_TYPE(const t, ::tt::member_object<int (t::*)(short)const>::type);
   BOOST_CHECK_TYPE(const t, ::tt::member_object<int (t::*const)(short)const>::type);
   BOOST_CHECK_TYPE(const t, ::tt::member_object<int (t::*const volatile)(short)const>::type);
   BOOST_CHECK_TYPE(const t, ::tt::member_object<int (t::*volatile)(short)const>::type);
}

TT_TEST_BEGIN(member_object)

   member_object_test_1();
   member_object_test_2();
   member_object_test_4();
   member_object_test_5();
   member_object_test_6();
   member_object_test_8();
   member_object_test_9();
   member_object_test_12();
   member_object_test_13();
   member_object_test_14();

TT_TEST_END

