// Numeric Adaptor Library testing suite
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/numeric_adaptor/numeric_adaptor.hpp>
#include <boost/numeric_adaptor/ieee_policy.hpp>

#if defined(HAVE_GMP)
#  include <boost/numeric_adaptor/gmp_policy.hpp>
#endif

#if defined(HAVE_CLN)
#  include <boost/numeric_adaptor/cln_policy.hpp>
#endif


template <typename Policy>
double heron()
{
    typedef boost::numeric_adaptor::numeric_adaptor<Policy> num;

    num a = 31622.77662;
    num b = 0.000023;
    num c = 31622.77661;
    num s((a + b + c) / num(2.0));
    return num::sqrt(s * (s - a) * (s - b) * (s - c));
}

int test_main(int, char*[])
{
    double epsilon = 0.0000001;

    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::ieee_policy<float> >(), 0.0, epsilon);
    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::ieee_policy<double> >(), 0.327490532778257, epsilon);
    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::ieee_policy<long double> >(), 0.327490459921098, epsilon);

#if defined(HAVE_CLN)
    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::cln_policy>(), 0.32749045994262366, epsilon);
#endif

#if defined(HAVE_GMP)
    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::gmp_policy>(), 0.327490459942623, epsilon);
#endif

    return 0;
};
