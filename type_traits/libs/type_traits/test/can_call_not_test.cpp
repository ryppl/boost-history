//  (C) Copyright 2009-2011 Frederic Bron.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/can_call_not.hpp>
#endif

#define BOOST_TT_TRAIT_NAME can_call_not
#define BOOST_TT_TRAIT_OP !

#include "can_call_prefix_operators.hpp"
#include "can_call_unary_plus_not_test.hpp"

#include <iostream>

namespace {
void specific() {
   TEST_TR(std::ostream, bool, true);
   TEST_TR(std::istream, bool, true);
}
}


TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)
   run1();
   run2();
   specific();
TT_TEST_END
