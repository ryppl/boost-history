//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <string>

// test with one template parameter
#define TEST_T(TYPE,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE>::value), RESULT)
// test with one template parameter plus return value
#define TEST_TR(TYPE,RET,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE,TYPE,RET>::value), RESULT)
// test with two template parameters
#define TEST_TT(TYPE1,TYPE2,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE1,TYPE2>::value), RESULT)
// test with two template parameters plus return value
#define TEST_TTR(TYPE1,TYPE2,RET,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE1,TYPE2,RET>::value), RESULT)

namespace {

struct ret { };

struct without { };

struct internal { ret operator BOOST_TT_TRAIT_OP (const internal&) const; };

struct external { };
ret operator BOOST_TT_TRAIT_OP (const external&, const external&);

class internal_private { ret operator BOOST_TT_TRAIT_OP (const internal_private&) const; };

struct returns_int { int operator BOOST_TT_TRAIT_OP (const returns_int&); };

struct returns_void { void operator BOOST_TT_TRAIT_OP (const returns_void&); };

struct returns_void_star { void *operator BOOST_TT_TRAIT_OP (const returns_void_star&); };

struct returns_double { double operator BOOST_TT_TRAIT_OP (const returns_double&); };

struct returns_string { std::string operator BOOST_TT_TRAIT_OP (const returns_string&); };

struct convertible_to_bool { operator bool () const; };
struct returns_convertible_to_bool { convertible_to_bool operator BOOST_TT_TRAIT_OP (const returns_convertible_to_bool&); };

class Base1 { };
class Derived1 : public Base1 { };

bool operator BOOST_TT_TRAIT_OP (const Base1&, const Base1&) { return true; }

class Base2 { };
struct Derived2 : public Base2 {
   Derived2(int); // to check if it works with a class that is not default constructible
};

bool operator BOOST_TT_TRAIT_OP (const Derived2&, const Derived2&) { return true; }

struct tag { };

struct A { };
struct B : public A { };

struct C { };
struct D { };
bool operator BOOST_TT_TRAIT_OP (const C&, void*) { return true; }
bool operator BOOST_TT_TRAIT_OP (void*, const D&) { return true; }
bool operator BOOST_TT_TRAIT_OP (const C&, const D&) { return true; }
}
