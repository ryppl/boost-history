//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

namespace {

void run2() {
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
   TEST_T(float, true);
   TEST_T(double, true);
   TEST_T(long double, true);
   TEST_T(void, false);
#	undef CV
#	define CV(T) const T
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) volatile T
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), true);
#	undef CV
#	define CV(T) const volatile T
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) const T&
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) volatile T&
   TEST_T(CV(bool), true);
   TEST_T(CV(int), true);
   TEST_T(CV(double), true);
#	undef CV
#	define CV(T) const volatile T&
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
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
   TEST_TR(float, bool, true);
   TEST_TR(double, bool, true);
   TEST_TR(long double, bool, true);
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

   // pointers
   TEST_T(void*, false);
   TEST_T(bool*, true);
   TEST_T(char*, true);
   TEST_T(int*, true);
   TEST_T(long*, true);
   TEST_T(wchar_t*, true);
   TEST_T(double*, true);
   TEST_T(without*, true);
}
}
