//  (C) Copyright Frederic Bron 2009-2011.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// test with one template parameter
#define TEST_T(TYPE,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE>::value), RESULT)
// test with one template parameter plus return value
#define TEST_TR(TYPE,RET,RESULT) BOOST_CHECK_INTEGRAL_CONSTANT((::tt::BOOST_TT_TRAIT_NAME<TYPE,RET>::value), RESULT)

namespace {

struct ret { };

struct without { };

struct internal { ret operator BOOST_TT_TRAIT_OP (int) const; };

struct external { };
ret operator BOOST_TT_TRAIT_OP (const external&, int);

struct returns_int { int operator BOOST_TT_TRAIT_OP (int); };

struct returns_void { void operator BOOST_TT_TRAIT_OP (int); };

struct returns_void_star { void *operator BOOST_TT_TRAIT_OP (int); };

struct returns_double { double operator BOOST_TT_TRAIT_OP (int); };

struct ret1 { };
struct convertible_to_ret1 { operator ret1 () const; };
struct returns_convertible_to_ret1 { convertible_to_ret1 operator BOOST_TT_TRAIT_OP (int); };

struct convertible_to_ret2 { };
struct ret2 { ret2(const convertible_to_ret2); };
struct returns_convertible_to_ret2 { convertible_to_ret2 operator BOOST_TT_TRAIT_OP (int); };

class Base1 { };
class Derived1 : public Base1 { };

bool operator BOOST_TT_TRAIT_OP (const Base1&, int) { return true; }

class Base2 { };
struct Derived2 : public Base2 {
   Derived2(int); // to check if it works with a class that is not default constructible
};

bool operator BOOST_TT_TRAIT_OP (const Derived2&, int) { return true; }

struct tag { };

//class internal_private { ret operator BOOST_TT_TRAIT_OP (int) const; };

void run1() {
   TEST_T(without, false);
   TEST_T(internal, true);
   TEST_T(external, true);
   TEST_T(returns_int, true);
   TEST_T(returns_void, true);
   TEST_T(returns_void_star, true);
   TEST_T(returns_double, true);
   TEST_T(returns_convertible_to_ret1, true);
   TEST_T(returns_convertible_to_ret2, true);
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
   TEST_TR(returns_convertible_to_ret1, ret1, true);
   TEST_TR(returns_convertible_to_ret2, ret2, true);
   TEST_TR(Base1, bool, true);
   TEST_TR(Derived1, bool, true);
   TEST_TR(Base2, bool, false);
   TEST_TR(Derived2, bool, true);
// compile time error
// TEST_T(internal_private, false);
}

}
