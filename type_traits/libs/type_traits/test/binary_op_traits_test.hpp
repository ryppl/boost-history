//  (C) Copyright Frederic Bron 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>

namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test) {

class Base1 { } ;
class Derived1 : public Base1 { } ;

bool operator BOOST_TT_TRAIT_OP (const Base1&, const Base1&) { return true ; }

class Base2 { } ;
struct Derived2 : public Base2 {
	Derived2(int) ; // to check if it works with a class that is not default constructible
} ;

bool operator BOOST_TT_TRAIT_OP (const Derived2&, const Derived2&) { return true ; }

class Without { } ;

struct member_comparable {
	bool operator BOOST_TT_TRAIT_OP (const member_comparable&) const ;
} ;

struct returns_void { } ;
void operator BOOST_TT_TRAIT_OP (const returns_void&, const returns_void&) ;

struct returns_void_star { } ;
void* operator BOOST_TT_TRAIT_OP (const returns_void_star&, const returns_void_star&) ;

struct returns_double { } ;
double operator BOOST_TT_TRAIT_OP (const returns_double&, const returns_double&) ;

struct returns_string { } ;
std::string operator BOOST_TT_TRAIT_OP (const returns_string&, const returns_string&) ;

}

TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)

BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<bool>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<char>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<signed char>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<unsigned char>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<wchar_t>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<signed wchar_t>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<unsigned wchar_t>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<short>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<unsigned short>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<unsigned int>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<long>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<unsigned long>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<float>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<double>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<std::string>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Base1>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Derived1>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Base2>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Derived2>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Without>::value, false);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::member_comparable>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_void>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_void_star>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_double>::value, true);
BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_string>::value, true);

TT_TEST_END
