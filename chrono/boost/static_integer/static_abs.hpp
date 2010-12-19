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

#ifndef BOOST_STATIC_INTEGER_STATIC_ABS_HPP
#define BOOST_STATIC_INTEGER_STATIC_ABS_HPP

#include <boost/cstdint.hpp>
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
    template <typename T, T X>
    struct static_abs 
        : integral_constant<T, 
            X < 0 ? -X : X> 
    {};

    typedef boost::intmax_t  static_abs_signed_type;
    typedef boost::uintmax_t  static_abs_unsigned_type;

    template <static_abs_signed_type X>
    struct static_signed_abs : static_abs<static_abs_signed_type, X>
        //~ : integral_constant<static_abs_signed_type, 
            //~ X < 0 ? -X : X> 
    {};

    template <static_abs_signed_type X>
    struct static_unsigned_abs : static_abs<static_abs_signed_type, X>
        //~ : integral_constant<static_abs_unsigned_type, 
            //~ X < 0 ? -X : X> 
    {};


}  
}
#endif
