//  (C) Copyright 2009-2011 Frederic Bron.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/can_call_left_shift.hpp>
#endif

#define BOOST_TT_TRAIT_NAME can_call_left_shift
#define BOOST_TT_TRAIT_OP <<

#include "can_call_binary_operators.hpp"
#include "can_call_integral_test.hpp"

#include <ostream>
#include <string>

#define TEST_SPECIFIC(T) TEST_TTR(std::ostream&, T, std::ostream&, true)
namespace {

void specific() {
   TEST_SPECIFIC(long);
   TEST_SPECIFIC(unsigned long);
   TEST_SPECIFIC(bool);
   TEST_SPECIFIC(short);
   TEST_SPECIFIC(unsigned short);
   TEST_SPECIFIC(int);
   TEST_SPECIFIC(unsigned int);
   TEST_SPECIFIC(double);
   TEST_SPECIFIC(float);
   TEST_SPECIFIC(void*);
   TEST_SPECIFIC(char);
   TEST_SPECIFIC(signed char);
   TEST_SPECIFIC(unsigned char);
   TEST_SPECIFIC(const char*);
   TEST_SPECIFIC(char*);
   TEST_SPECIFIC(const signed char*);
   TEST_SPECIFIC(signed char*);
   TEST_SPECIFIC(const unsigned char*);
   TEST_SPECIFIC(unsigned char*);
   TEST_SPECIFIC(std::string);
}
}


TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)
   run1();
   run2();
   specific();
TT_TEST_END
