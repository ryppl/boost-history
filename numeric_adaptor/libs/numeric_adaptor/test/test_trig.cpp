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
void test_all()
{
    typedef boost::numeric_adaptor::numeric_adaptor<Policy> num;

    // big epsilon since we're testing functionalities, not robustness
    double epsilon = 0.1;

    num n1 = 123.0;
    num n2 = 456.0;
    BOOST_CHECK_CLOSE((double)boost::numeric_adaptor::numeric_adaptor<Policy>::cos(n1), ::cos(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::numeric_adaptor::numeric_adaptor<Policy>::sin(n1), ::sin(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::numeric_adaptor::numeric_adaptor<Policy>::tan(n1), ::tan(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::numeric_adaptor::numeric_adaptor<Policy>::atan(n1), ::atan(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::numeric_adaptor::numeric_adaptor<Policy>::hypot(n1, n2), ::hypot(123.0, 456.0), epsilon);
}

int test_main(int, char*[])
{
    test_all<boost::numeric_adaptor::ieee_policy<float> >();
    test_all<boost::numeric_adaptor::ieee_policy<double> >();
    test_all<boost::numeric_adaptor::ieee_policy<long double> >();

#if defined(HAVE_GMP)
    test_all<boost::numeric_adaptor::gmp_policy>();
#endif

#if defined(HAVE_CLN)
    test_all<boost::numeric_adaptor::cln_policy>();
#endif

    return 0;
};
