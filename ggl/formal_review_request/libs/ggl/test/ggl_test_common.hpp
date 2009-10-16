// Generic Geometry Library test file
//
// Copyright Barend Gehrels 1995-2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2008, 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#ifndef GGL_TEST_GGL_TEST_COMMON_HPP
#define GGL_TEST_GGL_TEST_COMMON_HPP

// Just include some always-included files

#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/included/test_exec_monitor.hpp>


#if defined(HAVE_GMP)
#  include <boost/numeric_adaptor/gmp_value_type.hpp>
#endif
#if defined(HAVE_CLN)
#  include <boost/numeric_adaptor/cln_value_type.hpp>
#endif

#if defined(HAVE_CLN) || defined(HAVE_GMP)
#  include <boost/numeric_adaptor/numeric_adaptor.hpp>
#endif


#endif // GGL_TEST_GGL_TEST_COMMON_HPP
