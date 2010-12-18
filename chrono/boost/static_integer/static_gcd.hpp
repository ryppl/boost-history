//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2010.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/static_integer for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STATIC_INTEGER_STATIC_GCD_HPP
#define BOOST_STATIC_INTEGER_STATIC_GCD_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/cstdint.hpp>

//
// We simply cannot include this header on gcc without getting copious warnings of the kind:
//
// boost/integer.hpp:77:30: warning: use of C99 long long integer constant
//
// And yet there is no other reasonable implementation, so we declare this a system header
// to suppress these warnings.
//
#if defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

namespace boost
{

namespace integer
{

    typedef boost::intmax_t  static_gcd_signed_type;
    typedef boost::uintmax_t  static_gcd_unsigned_type;

    template <static_gcd_signed_type X, boost::intmax_t Y>
    struct static_signed_gcd 
        : static_signed_gcd<Y, X % Y>
    {};
    
    template <static_gcd_signed_type X>
    struct static_signed_gcd<X, 0> 
        : integral_constant<static_gcd_signed_type, 
            X> 
    {};

    template <static_gcd_signed_type X>
    struct static_signed_gcd<0, X> 
        : integral_constant<static_gcd_signed_type, 
            X> 
    {};

    template <static_gcd_unsigned_type X, boost::intmax_t Y>
    struct static_unsigned_gcd 
        : integral_constant<static_gcd_unsigned_type, 
            static_unsigned_gcd<Y, X % Y>::value>
    {};
    
    template <static_gcd_unsigned_type X>
    struct static_unsigned_gcd<X, 0> 
        : integral_constant<static_gcd_signed_type, 
            X> 
    {};
}
}


#endif
