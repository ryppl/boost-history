// Numeric Adaptor Library testing suite
//
// Copyright Barend Gehrels 2009, Geodan Holding B.V. Amsterdam, the Netherlands.
// Copyright Bruno Lalande 2009
// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)


#include <string>
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
    double epsilon = 0.0001;

    BOOST_CHECK_CLOSE((double)boost::to<ValueType>(std::string("123.456")), 123.456, epsilon);
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
