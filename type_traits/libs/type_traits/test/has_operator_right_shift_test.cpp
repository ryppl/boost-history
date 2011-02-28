//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/has_operator_right_shift.hpp>
#endif

#define BOOST_TT_TRAIT_NAME has_operator_right_shift
#define BOOST_TT_TRAIT_OP >>

#include "has_binary_operator_test.hpp"
#include "has_integral_operator_test.hpp"

#include <istream>

#define TEST_SPECIFIC(T) TEST_TTR(std::istream, T, std::istream, true)
namespace {

void specific() {
   TEST_SPECIFIC(bool&);
   TEST_SPECIFIC(short&);
   TEST_SPECIFIC(unsigned short&);
   TEST_SPECIFIC(int&);
   TEST_SPECIFIC(unsigned int&);
   TEST_SPECIFIC(long&);
   TEST_SPECIFIC(unsigned long&);
   TEST_SPECIFIC(float&);
   TEST_SPECIFIC(double&);
   TEST_SPECIFIC(void*&);
   TEST_SPECIFIC(char&);
   TEST_SPECIFIC(signed char&);
   TEST_SPECIFIC(unsigned char&);
   TEST_SPECIFIC(char*);
   TEST_SPECIFIC(signed char*);
   TEST_SPECIFIC(unsigned char*);
   TEST_SPECIFIC(std::string&);
}
}


TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)
   run1();
   run2();
   specific();
TT_TEST_END
