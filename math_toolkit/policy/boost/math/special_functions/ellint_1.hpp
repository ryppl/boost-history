//  Copyright (c) 2006 Xiaogang Zhang
//  Copyright (c) 2006 John Maddock
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  History:
//  XZ wrote the original of this file as part of the Google
//  Summer of Code 2006.  JM modified it to fit into the
//  Boost.Math conceptual framework better, and to ensure
//  that the code continues to work no matter how many digits
//  type T has.

#ifndef BOOST_MATH_ELLINT_1_HPP
#define BOOST_MATH_ELLINT_1_HPP

#include <boost/math/special_functions/ellint_rf.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/policy/error_handling.hpp>

// Elliptic integrals (complete and incomplete) of the first kind
// Carlson, Numerische Mathematik, vol 33, 1 (1979)

namespace boost { namespace math {

template <class T1, class T2, class Policy>
typename tools::promote_args<T1, T2>::type ellint_1(T1 k, T2 phi, const Policy& pol);

namespace detail{

// Elliptic integral (Legendre form) of the first kind
template <typename T, typename Policy>
T ellint_f_imp(T phi, T k, const Policy& pol)
{
    using namespace std;
    using namespace boost::math::tools;
    using namespace boost::math::constants;

    static const char* function = "boost::math::ellint_f<%1%>(%1%,%1%)";

    if (abs(k) > 1)
    {
       return policy::raise_domain_error<T>(function,
            "Got k = %1%, function requires |k| <= 1", k, pol);
    }

    bool invert = false;
    if(phi < 0)
    {
       BOOST_MATH_INSTRUMENT_CODE(phi);
       phi = fabs(phi);
       invert = true;
    }

    T result;

    if(phi >= tools::max_value<T>())
    {
       // Need to handle infinity as a special case:
       result = policy::raise_overflow_error<T>(function, 0, pol);
       BOOST_MATH_INSTRUMENT_CODE(result);
    }
    else if(phi > 1 / tools::epsilon<T>())
    {
       // Phi is so large that phi%pi is necessarily zero (or garbage),
       // just return the second part of the duplication formula:
       result = 2 * phi * ellint_k_imp(k, pol) / constants::pi<T>();
       BOOST_MATH_INSTRUMENT_CODE(result);
    }
    else
    {
       // Carlson's algorithm works only for |phi| <= pi/2,
       // use the integrand's periodicity to normalize phi
       //
       // Xiaogang's original code used a cast to long long here
       // but that fails if T has more digits than a long long,
       // so rewritten to use fmod instead:
       //
       T rphi = fmod(phi, constants::pi<T>() / 2);
       BOOST_MATH_INSTRUMENT_CODE(rphi);
       T m = 2 * (phi - rphi) / constants::pi<T>();
       BOOST_MATH_INSTRUMENT_CODE(m);
       int s = 1;
       if(fmod(m, T(2)) > 0.5)
       {
          m += 1;
          s = -1;
          rphi = constants::pi<T>() / 2 - rphi;
          BOOST_MATH_INSTRUMENT_CODE(rphi);
       }
       T sinp = sin(rphi);
       T cosp = cos(rphi);
       result = s * sinp * ellint_rf_imp(cosp * cosp, 1 - k * k * sinp * sinp, T(1), pol);
       BOOST_MATH_INSTRUMENT_CODE(result);
       if(m != 0)
       {
          result += m * ellint_k_imp(k, pol);
          BOOST_MATH_INSTRUMENT_CODE(result);
       }
    }
    return invert ? -result : result;
}

// Complete elliptic integral (Legendre form) of the first kind
template <typename T, typename Policy>
T ellint_k_imp(T k, const Policy& pol)
{
    using namespace std;
    using namespace boost::math::tools;

    static const char* function = "boost::math::ellint_k<%1%>(%1%)";

    if (abs(k) > 1)
    {
       return policy::raise_domain_error<T>(function,
            "Got k = %1%, function requires |k| <= 1", k, pol);
    }
    if (abs(k) == 1)
    {
       return policy::raise_overflow_error<T>(function, 0, pol);
    }

    T x = 0;
    T y = 1 - k * k;
    T z = 1;
    T value = ellint_rf_imp(x, y, z, pol);

    return value;
}

template <typename T, typename Policy>
inline typename tools::promote_args<T>::type ellint_1(T k, const Policy& pol, const mpl::true_&)
{
   typedef typename tools::promote_args<T>::type result_type;
   typedef typename policy::evaluation<result_type, Policy>::type value_type;
   return policy::checked_narrowing_cast<result_type, Policy>(detail::ellint_k_imp(static_cast<value_type>(k), pol), "boost::math::ellint_1<%1%>(%1%)");
}

template <class T1, class T2>
inline typename tools::promote_args<T1, T2>::type ellint_1(T1 k, T2 phi, const mpl::false_&)
{
   return boost::math::ellint_1(k, phi, policy::policy<>());
}

}

// Complete elliptic integral (Legendre form) of the first kind
template <typename T>
inline typename tools::promote_args<T>::type ellint_1(T k)
{
   return ellint_1(k, policy::policy<>());
}

// Elliptic integral (Legendre form) of the first kind
template <class T1, class T2, class Policy>
inline typename tools::promote_args<T1, T2>::type ellint_1(T1 k, T2 phi, const Policy& pol)
{
   typedef typename tools::promote_args<T1, T2>::type result_type;
   typedef typename policy::evaluation<result_type, Policy>::type value_type;
   return policy::checked_narrowing_cast<result_type, Policy>(detail::ellint_f_imp(static_cast<value_type>(phi), static_cast<value_type>(k), pol), "boost::math::ellint_1<%1%>(%1%,%1%)");
}

template <class T1, class T2>
inline typename tools::promote_args<T1, T2>::type ellint_1(T1 k, T2 phi)
{
   typedef typename policy::is_policy<T2>::type tag_type;
   return detail::ellint_1(k, phi, tag_type());
}

}} // namespaces

#endif // BOOST_MATH_ELLINT_1_HPP
