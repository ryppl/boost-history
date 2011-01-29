//  (C) Copyright 2009-2011 Frédéric Bron (frederic.bron@m4x.org)
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include "test.hpp"
#include "check_integral_constant.hpp"

#ifdef TEST_STD
#  include <type_traits>
#else
#  include <boost/type_traits/has_operator_multiplies_equal.hpp>
#endif

#define BOOST_TT_TRAIT_NAME has_operator_multiplies_equal
#define BOOST_TT_TRAIT_OP *=

#include "has_no_pointer_operator_test.hpp"
