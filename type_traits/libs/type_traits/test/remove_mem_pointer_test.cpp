
//  (C) Copyright John Maddock 2000. 
//  Use, modification and distribution are subject to the 
//  Boost Software License, Version 1.0. (See accompanying file 
//  LICENSE_1_0.txt or copy at http://www.tt.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_type.hpp"
#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/remove_member_pointer.hpp>
#endif

struct t{};

BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_1, ::tt::remove_member_pointer, const, const)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_2, ::tt::remove_member_pointer, volatile, volatile)
BOOST_DECL_TRANSFORM_TEST3(remove_member_pointer_test_3, ::tt::remove_member_pointer, (t::*))
BOOST_DECL_TRANSFORM_TEST0(remove_member_pointer_test_4, ::tt::remove_member_pointer)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_5, ::tt::remove_member_pointer, const &, const&)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_6, ::tt::remove_member_pointer, &, &)
BOOST_DECL_TRANSFORM_TEST3(remove_member_pointer_test_7, ::tt::remove_member_pointer, (t::*volatile))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_8, ::tt::remove_member_pointer, const [2], const[2])
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_9, ::tt::remove_member_pointer, const &, const&)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_10, ::tt::remove_member_pointer, const (t::*), const)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_11, ::tt::remove_member_pointer, volatile (t::*), volatile)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_12, ::tt::remove_member_pointer, const[2][3], const[2][3])
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_13, ::tt::remove_member_pointer, (&)[2], (&)[2])
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_14, ::tt::remove_member_pointer, * (t::*), *)
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_15, ::tt::remove_member_pointer, const (t::*)[2], const[2])
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_16, ::tt::remove_member_pointer, (t::*)(double), (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_17, ::tt::remove_member_pointer, (t::*const)(double), (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_18, ::tt::remove_member_pointer, (t::*volatile)(double), (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_19, ::tt::remove_member_pointer, (t::*const volatile)(double), (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_20, ::tt::remove_member_pointer, (t::*)(double)const, (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_21, ::tt::remove_member_pointer, (t::*const)(double)volatile, (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_22, ::tt::remove_member_pointer, (t::*volatile)(double)const volatile, (double))
BOOST_DECL_TRANSFORM_TEST(remove_member_pointer_test_23, ::tt::remove_member_pointer, (t::*const volatile)(double)const, (double))

void remove_member_pointer_test_24()
{
#ifdef BOOST_MSVC
   typedef int f1(short);
   BOOST_CHECK_TYPE(f1, ::tt::remove_member_pointer<f1 (t::*)>::type);
   typedef int __fastcall f2(short);
   BOOST_CHECK_TYPE(f2, ::tt::remove_member_pointer<f2 (t::*)>::type);
   typedef int __cdecl f3(short);
   BOOST_CHECK_TYPE(f3, ::tt::remove_member_pointer<f3 (t::*)>::type);
#endif
}

TT_TEST_BEGIN(remove_member_pointer)

   remove_member_pointer_test_1();
   remove_member_pointer_test_2();
   remove_member_pointer_test_3();
   remove_member_pointer_test_4();
   remove_member_pointer_test_5();
   remove_member_pointer_test_6();
   remove_member_pointer_test_7();
   remove_member_pointer_test_8();
   remove_member_pointer_test_9();
   remove_member_pointer_test_10();
   remove_member_pointer_test_11();
   remove_member_pointer_test_12();
   remove_member_pointer_test_13();
   remove_member_pointer_test_14();
   remove_member_pointer_test_15();
   remove_member_pointer_test_16();
   remove_member_pointer_test_17();
   remove_member_pointer_test_18();
   remove_member_pointer_test_19();
   remove_member_pointer_test_20();
   remove_member_pointer_test_21();
   remove_member_pointer_test_22();
   remove_member_pointer_test_23();
   remove_member_pointer_test_24();

TT_TEST_END

