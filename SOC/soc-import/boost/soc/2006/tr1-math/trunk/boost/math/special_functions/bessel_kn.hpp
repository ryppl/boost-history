//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_BESSEL_KN_HPP
#define BOOST_MATH_BESSEL_KN_HPP

#ifndef BOOST_MATH_THROW_ON_DOMAIN_ERROR
#define BOOST_MATH_THROW_ON_DOMAIN_ERROR
#endif

#include <boost/math/special_functions/bessel_k0.hpp>
#include <boost/math/special_functions/bessel_k1.hpp>
#include <boost/math/tools/error_handling.hpp>

// Modified Bessel function of the second kind of integer order
// K_n(z) is the dominant solution, forward recurrence always OK (though unstable)

namespace boost { namespace math {

template <typename T>
T bessel_kn(int n, T x)
{
    T value, current, prev;

    using namespace boost::math::tools;

    if (x < 0)
    {
        domain_error<T>("boost::math::bessel_kn(n, x)",
            "domain error, real argument x must be non-negative, complex number not supported");
    }
    if (x == 0)
    {
        if (std::numeric_limits<T>::has_infinity)
        {
            return std::numeric_limits<T>::infinity();
        }
        else
        {
            overflow_error<T>("boost::math::bessel_kn(n, x)",
                              "infinity occurred but not supported");
        }
    }

    if (n == 0)
    {
        return bessel_k0(x);
    }
    if (n == 1)
    {
        return bessel_k1(x);
    }
    if (n < 0)
    {
        n = -n;                             // K_{-n}(z) = K_n(z)
    }

    prev = bessel_k0(x);
    current = bessel_k1(x);
    for (int k = 1; k < n; k++)            // n >= 2
    {
        value = 2.0L * k * current / x + prev;
        prev = current;
        current = value;
    }

    return value;
}

template <typename T>
inline T bessel_kn(T n, T x)
{
    return bessel_kn(static_cast<int>(n), x);
}

}} // namespaces

#endif // BOOST_MATH_BESSEL_KN_HPP
