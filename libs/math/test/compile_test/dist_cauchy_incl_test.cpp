//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/distributions/cauchy.hpp>
// #includes all the files that it needs to.
//
#define BOOST_MATH_ASSERT_UNDEFINED_POLICY false
#include <boost/math/distributions/cauchy.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void check()
{
   TEST_DIST_FUNC(cauchy)
}

template class boost::math::cauchy_distribution<float, boost::math::policies::policy<> >;
template class boost::math::cauchy_distribution<double, boost::math::policies::policy<> >;
template class boost::math::cauchy_distribution<long double, boost::math::policies::policy<> >;
