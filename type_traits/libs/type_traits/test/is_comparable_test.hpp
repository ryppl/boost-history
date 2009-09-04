//  (C) Copyright Frederic Bron 2009.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <typeinfo>
#include <string>
#include <vector>

#define TEST1(TYPE,RESULT)\
	BOOST_CHECK_INTEGRAL_CONSTANT(::tt::BOOST_TT_TRAIT_NAME<TYPE>::value, RESULT)

#define TEST2(TYPE1,TYPE2,RESULT)\
	BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE1,TYPE2>::value), RESULT);\
	BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE2,TYPE1>::value), RESULT)

namespace BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test) {

struct NotComparable { } ;
struct Comparable { bool operator BOOST_TT_TRAIT_OP (const Comparable&) const ; } ;
struct IntComparable { int operator BOOST_TT_TRAIT_OP (const IntComparable&) ; } ;
struct PtrComparable { void *operator BOOST_TT_TRAIT_OP (const PtrComparable&) ; } ;
struct Convertible { operator bool () const ; } ;
struct StrangeComparable { Convertible operator BOOST_TT_TRAIT_OP (const StrangeComparable&) ; } ;

class Base1 { } ;
class Derived1 : public Base1 { } ;

bool operator BOOST_TT_TRAIT_OP (const Base1&, const Base1&) { return true ; }

class Base2 { } ;
struct Derived2 : public Base2 {
	Derived2(int) ; // to check if it works with a class that is not default constructible
} ;

bool operator BOOST_TT_TRAIT_OP (const Derived2&, const Derived2&) { return true ; }

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

// built-in numeric types
	TEST1(bool, true) ;
	TEST1(char, true) ;
	TEST1(signed char, true) ;
	TEST1(unsigned char, true) ;
	TEST1(wchar_t, true) ;
	TEST1(short, true) ;
	TEST1(unsigned short, true) ;
	TEST1(int, true) ;
	TEST1(unsigned int, true) ;
	TEST1(long, true) ;
	TEST1(unsigned long, true) ;
	TEST1(float, true) ;
	TEST1(double, true) ;
	TEST1(std::string, true) ;
// built-in different numeric types
	TEST2(bool,char, true) ;
	TEST2(bool,signed char, true) ;
	TEST2(bool,unsigned char, true) ;
	TEST2(bool,wchar_t, true) ;
	TEST2(bool,short, true) ;
	TEST2(bool,unsigned short, true) ;
	TEST2(bool,int, true) ;
	TEST2(bool,unsigned int, true) ;
	TEST2(bool,long, true) ;
	TEST2(bool,unsigned long, true) ;
	TEST2(bool,float, true) ;
	TEST2(bool,double, true) ;

	TEST2(char,signed char, true) ;
	TEST2(char,unsigned char, true) ;
	TEST2(char,wchar_t, true) ;
	TEST2(char,short, true) ;
	TEST2(char,unsigned short, true) ;
	TEST2(char,int, true) ;
	TEST2(char,unsigned int, true) ;
	TEST2(char,long, true) ;
	TEST2(char,unsigned long, true) ;
	TEST2(char,float, true) ;
	TEST2(char,double, true) ;

	TEST2(signed char,unsigned char, true) ;
	TEST2(signed char,wchar_t, true) ;
	TEST2(signed char,short, true) ;
	TEST2(signed char,unsigned short, true) ;
	TEST2(signed char,int, true) ;
	TEST2(signed char,unsigned int, true) ;
	TEST2(signed char,long, true) ;
	TEST2(signed char,unsigned long, true) ;
	TEST2(signed char,float, true) ;
	TEST2(signed char,double, true) ;

	TEST2(unsigned char,wchar_t, true) ;
	TEST2(unsigned char,short, true) ;
	TEST2(unsigned char,unsigned short, true) ;
	TEST2(unsigned char,int, true) ;
	TEST2(unsigned char,unsigned int, true) ;
	TEST2(unsigned char,long, true) ;
	TEST2(unsigned char,unsigned long, true) ;
	TEST2(unsigned char,float, true) ;
	TEST2(unsigned char,double, true) ;

	TEST2(wchar_t,short, true) ;
	TEST2(wchar_t,unsigned short, true) ;
	TEST2(wchar_t,int, true) ;
	TEST2(wchar_t,unsigned int, true) ;
	TEST2(wchar_t,long, true) ;
	TEST2(wchar_t,unsigned long, true) ;
	TEST2(wchar_t,float, true) ;
	TEST2(wchar_t,double, true) ;

	TEST2(short,unsigned short, true) ;
	TEST2(short,int, true) ;
	TEST2(short,unsigned int, true) ;
	TEST2(short,long, true) ;
	TEST2(short,unsigned long, true) ;
	TEST2(short,float, true) ;
	TEST2(short,double, true) ;

	TEST2(unsigned short,int, true) ;
	TEST2(unsigned short,unsigned int, true) ;
	TEST2(unsigned short,long, true) ;
	TEST2(unsigned short,unsigned long, true) ;
	TEST2(unsigned short,float, true) ;
	TEST2(unsigned short,double, true) ;

	TEST2(int,unsigned int, true) ;
	TEST2(int,long, true) ;
	TEST2(int,unsigned long, true) ;
	TEST2(int,float, true) ;
	TEST2(int,double, true) ;

	TEST2(unsigned int,long, true) ;
	TEST2(unsigned int,unsigned long, true) ;
	TEST2(unsigned int,float, true) ;
	TEST2(unsigned int,double, true) ;

	TEST2(long,unsigned long, true) ;
	TEST2(long,float, true) ;
	TEST2(long,double, true) ;

	TEST2(unsigned long,float, true) ;
	TEST2(unsigned long,double, true) ;

	TEST2(float,double, true) ;

	TEST2(double, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::NotComparable, false) ;
	TEST2(int, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::NotComparable, false) ;
	TEST2(double, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::IntComparable, false) ;
	TEST2(int, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::IntComparable, false) ;
	TEST2(double, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Comparable, false) ;
	TEST2(int, BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Comparable, false) ;

	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::NotComparable, false) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Comparable, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::IntComparable, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::PtrComparable, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::StrangeComparable, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Base1, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Derived1, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Base2, false) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::Derived2, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::member_comparable, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_void_star, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_double, true) ;
	TEST1(BOOST_JOIN(BOOST_TT_TRAIT_NAME,_test)::returns_string, false) ;

TT_TEST_END
