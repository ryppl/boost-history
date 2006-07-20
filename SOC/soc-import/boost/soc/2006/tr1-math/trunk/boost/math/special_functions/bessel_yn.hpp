//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_YN_HPP
#define BOOST_MATH_BESSEL_YN_HPP
#endif

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/special_functions/bessel_y0.hpp>
#include <boost/math/special_functions/bessel_y1.hpp>
#include <boost/math/tools/error_handling.hpp>

// Bessel function of the second kind of integer order
// Y_n(z) is the dominant solution, forward recurrence always OK

namespace boost { namespace math {

template <typename T>
T bessel_yn(int n, T x)
{
    T value, factor, current, prev;

    using namespace boost::math::tools;

    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_yn(n, x)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            return -std::numeric_limits<T>::infinity();
        }
        else
        {
            overflow_error<T>("boost::math::bessel_yn(n, x)",
                              "infinity occurred but not supported");
        }
    }

    if (n == 0)
    {
        return bessel_y0(x);
    }
    if (n == 1)
    {
        return bessel_y1(x);
    }
    if (n < 0)
    {
        factor = (n & 0x1) ? -1.0L : 1.0L;  // Y_{-n}(z) = (-1)^n Y_n(z)
        n = -n;
    }
    else
    {
        factor = 1.0L;
    }

    prev = bessel_y0(x);
    current = bessel_y1(x);
    for (int k = 1; k < n; k++)            // n >= 2
    {
        value = 2.0L * k * current / x - prev;
        prev = current;
        current = value;
    }
    value *= factor;

    return value;
}

template <class T>
inline T bessel_yn(T n, T x)
{
    return bessel_yn(static_cast<int>(n), x);
}

}} // namespaces
