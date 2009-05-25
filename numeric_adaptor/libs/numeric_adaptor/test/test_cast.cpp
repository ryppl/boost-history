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
#ifndef NO_CLN
#include <boost/numeric_adaptor/cln_policy.hpp>
#endif
#include <boost/numeric_adaptor/gmp_policy.hpp>


template <typename Policy>
void test_all()
{
    typedef numeric_adaptor::numeric_adaptor<Policy> num;

    num n = 1234.5;
    BOOST_CHECK_EQUAL(int(n), 1234);
    BOOST_CHECK_EQUAL(float(n), 1234.5);
    BOOST_CHECK_EQUAL(double(n), 1234.5);
    BOOST_CHECK_EQUAL(n.template get<std::string>(), "1234.5");

    num n2 = "1234.5";
    BOOST_CHECK_EQUAL(double(n2), 1234.5);

    num n3("1234.5");
    BOOST_CHECK_EQUAL(double(n3), 1234.5);
}

int test_main(int, char*[])
{
    test_all<numeric_adaptor::ieee_policy<double> >();

    return 0;
};
