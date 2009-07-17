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

#if defined(HAVE_GMP)
#  include <boost/numeric_adaptor/gmp_value_type.hpp>
#endif

#if defined(HAVE_CLN)
#  include <boost/numeric_adaptor/cln_value_type.hpp>
#endif


template <typename ValueType>
void test_all()
{
    // big epsilon since we're testing functionalities, not robustness
    double epsilon = 0.1;

    ValueType n1 = 123.0;
    ValueType n2 = 456.0;
    BOOST_CHECK_CLOSE((double)boost::cos(n1), ::cos(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::sin(n1), ::sin(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::tan(n1), ::tan(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::atan(n1), ::atan(123.0), epsilon);
    BOOST_CHECK_CLOSE((double)boost::hypot(n1, n2), ::hypot(123.0, 456.0), epsilon);
}

int test_main(int, char*[])
{
    test_all<float>();
    test_all<double>();
    test_all<long double>();

#if defined(HAVE_GMP)
    test_all<boost::numeric_adaptor::gmp_value_type>();
#endif

#if defined(HAVE_CLN)
    test_all<boost::numeric_adaptor::cln_value_type>();
#endif

    return 0;
};
