//  (C) Copyright Frederic Bron 2009-2010.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <typeinfo>
#include <string>

// test with one template parameter
#define TEST_T(TYPE,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE>::value), RESULT)
// test with one template parameter plus return value
#define TEST_TR(TYPE,RET,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE,RET>::value), RESULT)

namespace {

struct ret { };

struct without { };

struct internal { ret operator BOOST_TT_TRAIT_OP () const; };

struct external { };
ret operator BOOST_TT_TRAIT_OP (const external&);

class internal_private { ret operator BOOST_TT_TRAIT_OP () const; };

struct returns_int { int operator BOOST_TT_TRAIT_OP (); };

struct returns_void { void operator BOOST_TT_TRAIT_OP (); };

struct returns_void_star { void *operator BOOST_TT_TRAIT_OP (); };

struct returns_double { double operator BOOST_TT_TRAIT_OP (); };

struct returns_string { std::string operator BOOST_TT_TRAIT_OP (); };

//struct convertible_to_bool { operator bool () const; };
//struct returns_convertible_to_bool { convertible_to_bool operator BOOST_TT_TRAIT_OP (); };

class Base1 { };
class Derived1 : public Base1 { };

bool operator BOOST_TT_TRAIT_OP (const Base1&) { return true; }

class Base2 { };
struct Derived2 : public Base2 {
	Derived2(int); // to check if it works with a class that is not default constructible
};

bool operator BOOST_TT_TRAIT_OP (const Derived2&) { return true; }

struct tag { };

void run() {
	// test with only one template parameter
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

	TEST_T(without, false);
	TEST_T(internal, true);
	TEST_T(external, true);
//	compile time error
//	TEST_T(internal_private, false);
	TEST_T(returns_int, true);
	TEST_T(returns_void, true);
	TEST_T(returns_void_star, true);
	TEST_T(returns_double, true);
	TEST_T(returns_string, true);
//	TEST_T(convertible_to_bool, true);
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
//	TEST_TR(convertible_to_bool, bool, true);
	TEST_TR(Base1, bool, true);
	TEST_TR(Derived1, bool, true);
	TEST_TR(Base2, bool, false);
	TEST_TR(Derived2, bool, true);
}
}

TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)
	run();
TT_TEST_END
