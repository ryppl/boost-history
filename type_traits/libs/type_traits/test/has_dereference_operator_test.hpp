//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

namespace {

void run2() {
   // test with only one template parameter
   TEST_T(void, false);
   TEST_T(bool, false);
   TEST_T(char, false);
   TEST_T(signed char, false);
   TEST_T(short int, false);
   TEST_T(int, false);
   TEST_T(long int, false);
   TEST_T(unsigned char, false);
   TEST_T(unsigned short int, false);
   TEST_T(unsigned int, false);
   TEST_T(unsigned long int, false);
   TEST_T(wchar_t, false);
   TEST_T(float, false);
   TEST_T(double, false);
   TEST_T(long double, false);
   TEST_T(void, false);
#	undef CV
#	define CV(T) const T
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) volatile T
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);
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
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);
#	undef CV
#	define CV(T) const volatile T&
   TEST_T(CV(bool), false);
   TEST_T(CV(int), false);
   TEST_T(CV(double), false);

   // test with three template parameters
   TEST_TR(bool, bool, false);
   TEST_TR(char, bool, false);
   TEST_TR(signed char, bool, false);
   TEST_TR(short int, bool, false);
   TEST_TR(int, bool, false);
   TEST_TR(long int, bool, false);
   TEST_TR(unsigned char, bool, false);
   TEST_TR(unsigned short int, bool, false);
   TEST_TR(unsigned int, bool, false);
   TEST_TR(unsigned long int, bool, false);
   TEST_TR(wchar_t, bool, false);
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

   // pointers
   TEST_T(void*, false);
   TEST_T(bool*, true);
   TEST_T(char*, true);
   TEST_T(int*, true);
   TEST_T(long*, true);
   TEST_T(wchar_t*, true);
   TEST_T(double*, true);
   TEST_T(without*, true);
   TEST_TR(char*, char, true);
   TEST_TR(const char*, char, true);
   TEST_TR(const char*, char&, false);
   TEST_TR(char*, const char, true);
}
}
