//  (C) Copyright 2009-2011 Frederic Bron.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/can_call_subtraction.hpp>
#endif

#define BOOST_TT_TRAIT_NAME can_call_subtraction
#define BOOST_TT_TRAIT_OP -

#include "can_call_binary_operators.hpp"
#include "can_call_subtraction_test.hpp"

namespace {
void specific() {
   /* nothing specific */
}
}



TT_TEST_BEGIN(BOOST_TT_TRAIT_NAME)
   run1();
   run2();
   specific();
TT_TEST_END
