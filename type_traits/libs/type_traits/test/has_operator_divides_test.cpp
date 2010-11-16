//  (C) Copyright Frederic Bron 2010.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/has_operator_divides.hpp>
#endif

#define BOOST_TT_TRAIT_NAME has_operator_divides
#define BOOST_TT_TRAIT_OP /

#include "has_binary_operator_test.hpp"
