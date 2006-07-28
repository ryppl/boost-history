//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_JN_HPP
#define BOOST_MATH_BESSEL_JN_HPP

#include <boost/math/special_functions/bessel_j0.hpp>
#include <boost/math/special_functions/bessel_j1.hpp>

// Bessel function of the first kind of integer order
// J_n(z) is the minimal solution
// n < abs(z), forward recurrence stable and usable
// n >= abs(z), forward recurrence unstable, use Miller's algorithm

namespace boost { namespace math {

template <typename T>
T bessel_jn(int n, T x)
{
    T value, factor, current, prev, next;

    using namespace std;

    if (n == 0)
    {
        return bessel_j0(x);
    }
    if (n == 1)
    {
        return bessel_j1(x);
    }
    if (n < 0)
    {
        factor = (n & 0x1) ? -1.0L : 1.0L;  // J_{-n}(z) = (-1)^n J_n(z)
        n = -n;
    }
    else
    {
        factor = 1.0L;
    }

    if (x == 0)                             // n >= 2
    {
        return static_cast<T>(0);
    }

    if (n < abs(x))                         // forward recurrence
    {
        prev = bessel_j0(x);
        current = bessel_j1(x);
        for (int k = 1; k < n; k++)
        {
            value = 2.0L * k * current / x - prev;
            prev = current;
            current = value;
        }
    }
    else                                    // backward recurrence
    {
        int m = n + sqrt(50.0L * n);        // hard-coded constant, changeable
        prev = 0.0L;
        current = 1.0L;
        for (int k = m; k > 0; k--)
        {
            if (k == n)
            {
                value = current;
            }
            next = 2.0L * k * current / x - prev;
            prev = current;
            current = next;
        }
        if (abs(current) > abs(prev))       // normalization
        {
            value *= bessel_j0(x) / current;
        }
        else
        {
            value *= bessel_j1(x) / prev;
        }
    }
    value *= factor;

    return value;
}

template <class T>
inline T bessel_jn(T n, T x)
{
    return bessel_jn(static_cast<int>(n), x);
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_JN_HPP
