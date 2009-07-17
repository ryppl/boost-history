// Numeric Adaptor Library testing suite
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/test/included/test_exec_monitor.hpp>
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
    ValueType n1 = 1.0;
    ValueType n2 = 2.0;

    ValueType n3 = n1 + n2;
    BOOST_CHECK_EQUAL((double)n3, 3.0);
    n3 += 4.0;
    BOOST_CHECK_EQUAL((double)n3, 7.0);

    ValueType n4 = n3 - n1;
    BOOST_CHECK_EQUAL((double)n4, 6.0);
    n4 -= 2.0;
    BOOST_CHECK_EQUAL((double)n4, 4.0);

    ValueType n5 = n4*n3;
    BOOST_CHECK_EQUAL((double)n5, 28.0);
    n5 *= n2;
    BOOST_CHECK_EQUAL((double)n5, 56.0);

    ValueType n6 = n5/n4;
    BOOST_CHECK_EQUAL((double)n6, 14.0);
    n6 /= n3;
    BOOST_CHECK_EQUAL((double)n6, 2.0);

    BOOST_CHECK_EQUAL((double)-n1, -1.0);
    BOOST_CHECK_EQUAL((double)-n2, -2.0);
    BOOST_CHECK_EQUAL((double)-n3, -7.0);
    BOOST_CHECK_EQUAL((double)-n4, -4.0);
    BOOST_CHECK_EQUAL((double)-n5, -56.0);
    BOOST_CHECK_EQUAL((double)-n6, -2.0);

    BOOST_CHECK_EQUAL((double)abs(-n5), 56.0);
    BOOST_CHECK_EQUAL((double)sqrt(n4), 2.0);
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
