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

#ifndef BOOST_STATIC_INTEGER_STATIC_LCM_HPP
#define BOOST_STATIC_INTEGER_STATIC_LCM_HPP

#include <boost/cstdint.hpp>
#include <boost/static_integer/static_gcd.hpp>
#include <boost/static_integer/static_types.hpp>
#include <boost/type_traits/integral_constant.hpp>

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

    template <typename T, T X, T Y>
    struct static_lcm 
        : integral_constant<T, 
            X / static_gcd<T, X, Y>::value * Y>
    {};
    
    template <static_signed_type X, static_signed_type Y>
    struct static_signed_lcm : static_lcm<static_signed_type, X, Y>
    {};

    template <static_unsigned_type X, static_unsigned_type Y>
    struct static_unsigned_lcm : static_lcm<static_unsigned_type, X, Y>
    {};

}
}


#endif
