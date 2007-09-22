//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Basic sanity check that header <boost/math/distributions/beta.hpp>
// #includes all the files that it needs to.
//
#include <boost/math/distributions/beta.hpp>
//
// Note this header includes no other headers, this is
// important if this test is to be meaningful:
//
#include "test_compile_result.hpp"

void check()
{
   TEST_DIST_FUNC(beta)
}

template class boost::math::beta_distribution<double, boost::math::policies::policy<> >;
template class boost::math::beta_distribution<float, boost::math::policies::policy<> >;
template class boost::math::beta_distribution<long double, boost::math::policies::policy<> >;

