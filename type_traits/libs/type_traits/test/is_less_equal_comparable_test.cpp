//  (C) Copyright Frederic Bron 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"
#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/is_less_equal_comparable.hpp>
#endif

#define BOOST_TT_TRAIT_NAME is_less_equal_comparable
#define BOOST_TT_TRAIT_OP <=

#include "is_comparable_test.hpp"