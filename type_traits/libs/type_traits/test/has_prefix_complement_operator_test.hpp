//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "has_prefix_operator_test.hpp"

namespace {

void run() {
   // test with only one template parameter
   TEST_T(void, false);
   TEST_T(bool, true);
   TEST_T(char, true);
   TEST_T(signed char, true);
   TEST_T(short int, true);
   TEST_T(int, true);
   TEST_T(long int, true);
   TEST_T(unsigned char, true);
   TEST_T(unsigned short int, true);
   TEST_T(unsigned int, true);
   TEST_T(unsigned long int, true);
   TEST_T(wchar_t, true);
   TEST_T(float, false);
   TEST_T(double, false);
   TEST_T(long double, false);
   TEST_T(void, false);
#	undef CV
#	define CV(T) const T
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) volatile T
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) const volatile T
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) const T&
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) volatile T&
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) const volatile T&
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), false);

   // test with three template parameters
   TEST_TR(bool, bool, true);
   TEST_TR(char, bool, true);
   TEST_TR(signed char, bool, true);
   TEST_TR(short int, bool, true);
   TEST_TR(int, bool, true);
   TEST_TR(long int, bool, true);
   TEST_TR(unsigned char, bool, true);
   TEST_TR(unsigned short int, bool, true);
   TEST_TR(unsigned int, bool, true);
   TEST_TR(unsigned long int, bool, true);
   TEST_TR(wchar_t, bool, true);
   TEST_TR(float, bool, false);
   TEST_TR(double, bool, false);
   TEST_TR(long double, bool, false);
   TEST_TR(bool, tag, false);
   TEST_TR(char, tag, false);
   TEST_TR(signed char, tag, false);
   TEST_TR(short int, tag, false);
   TEST_TR(int, tag, false);
   TEST_TR(long int, tag, false);
   TEST_TR(unsigned char, tag, false);
   TEST_TR(unsigned short int, tag, false);
   TEST_TR(unsigned int, tag, false);
   TEST_TR(unsigned long int, tag, false);
   TEST_TR(wchar_t, tag, false);
   TEST_TR(float, tag, false);
   TEST_TR(double, tag, false);
   TEST_TR(long double, tag, false);

   TEST_T(without, false);
   TEST_T(internal, true);
   TEST_T(external, true);
// compile time error
// TEST_T(internal_private, false);
   TEST_T(returns_int, true);
   TEST_T(returns_void, true);
   TEST_T(returns_void_star, true);
   TEST_T(returns_double, true);
   TEST_T(returns_string, true);
   TEST_T(returns_convertible_to_bool, true);
   TEST_T(Base1, true);
   TEST_T(Derived1, true);
   TEST_T(Base2, false);
   TEST_T(Derived2, true);

   TEST_TR(without, bool, false);
   TEST_TR(internal, bool, false);
   TEST_TR(internal, ret, true);
   TEST_TR(external, bool, false);
   TEST_TR(external, ret, true);
   TEST_TR(returns_int, bool, true);
   TEST_TR(returns_int, int, true);
   TEST_TR(returns_void, void, true);
   TEST_TR(returns_void, bool, false);
   TEST_TR(returns_void_star, bool, true);
   TEST_TR(returns_double, bool, true);
   TEST_TR(returns_double, double, true);
   TEST_TR(returns_string, bool, false);
   TEST_TR(returns_string, std::string, true);
   TEST_TR(returns_convertible_to_bool, bool, true);
   TEST_TR(Base1, bool, true);
   TEST_TR(Derived1, bool, true);
   TEST_TR(Base2, bool, false);
   TEST_TR(Derived2, bool, true);
   // pointers
   TEST_T(void*, false);
   TEST_T(bool*, false);
   TEST_T(char*, false);
   TEST_T(int*, false);
   TEST_T(long*, false);
   TEST_T(wchar_t*, false);
   TEST_T(double*, false);
   TEST_T(without*, false);
}
}

TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)
   run();
TT_TEST_END
