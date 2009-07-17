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
double heron()
{
    ValueType a = 31622.77662;
    ValueType b = 0.000023;
    ValueType c = 31622.77661;
    ValueType s((a + b + c) / ValueType(2.0));
    return boost::sqrt(s * (s - a) * (s - b) * (s - c));
}

int test_main(int, char*[])
{
    double epsilon = 0.0000001;

    BOOST_CHECK_CLOSE(heron<float>(), 0.0, epsilon);
    BOOST_CHECK_CLOSE(heron<double>(), 0.32749040502871557, epsilon);
    BOOST_CHECK_CLOSE(heron<long double>(), 0.327490459921098, epsilon);

#if defined(HAVE_CLN)
    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::cln_value_type>(), 0.32749045994262366, epsilon);
#endif

#if defined(HAVE_GMP)
    BOOST_CHECK_CLOSE(heron<boost::numeric_adaptor::gmp_value_type>(), 0.327490459942623, epsilon);
#endif

    return 0;
};
