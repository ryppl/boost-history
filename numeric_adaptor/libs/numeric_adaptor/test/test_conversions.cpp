// Numeric Adaptor Library testing suite
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <boost/test/included/test_exec_monitor.hpp>
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

    num n = 1234.5;
    BOOST_CHECK_EQUAL(int(n), 1234);
    BOOST_CHECK_EQUAL((unsigned int)(n), 1234);
    BOOST_CHECK_EQUAL((unsigned short)(n), 1234);
    BOOST_CHECK_EQUAL((unsigned long)(n), 1234);
    BOOST_CHECK_EQUAL(float(n), 1234.5);
    BOOST_CHECK_EQUAL(double(n), 1234.5);
    BOOST_CHECK_EQUAL(std::string(n), "1234.5");

    num n2 = "1234.5";
    BOOST_CHECK_EQUAL(double(n2), 1234.5);

    num n3("1234.5");
    BOOST_CHECK_EQUAL(double(n3), 1234.5);
}

int test_main(int, char*[])
{
    using namespace boost::numeric_adaptor;
    test_all<ieee_policy<double> >();
#if defined(HAVE_GMP)
    test_all<gmp_policy>();
#endif

#if defined(HAVE_CLN)
    test_all<cln_policy>();
#endif

    return 0;
};
