//  Copyright John Maddock 2008.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0.  (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#  include <pch.hpp>
#ifndef BOOST_MATH_TR1_SOURCE
#  define BOOST_MATH_TR1_SOURCE
#endif
#include <boost/math/tr1.hpp>
#include <boost/math/special_functions/hermite.hpp>
#include "c_policy.hpp"

extern "C" long double BOOST_MATH_TR1_DECL hermitel BOOST_PREVENT_MACRO_SUBSTITUTION(unsigned n, long double x) BOOST_MATH_C99_THROW_SPEC
{
   return c_policies::hermite BOOST_PREVENT_MACRO_SUBSTITUTION(n, x);
}


