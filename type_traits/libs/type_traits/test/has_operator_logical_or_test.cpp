//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/has_operator_logical_or.hpp>
#endif

#define BOOST_TT_TRAIT_NAME has_operator_logical_or
#define BOOST_TT_TRAIT_OP ||

#include "has_binary_operator_test.hpp"
#include "has_binary_logical_operator_test.hpp"

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
