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

#ifndef BOOST_STATIC_INTEGER_STATIC_SIGN_HPP
#define BOOST_STATIC_INTEGER_STATIC_SIGN_HPP

#include <boost/type_traits/integral_constant.hpp>
#include <boost/static_integer/static_types.hpp>
#include <boost/mpl/sign.hpp>

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
    struct static_sign 
        : mpl::sign< integral_constant<T,X> >
    {
    };
    
    template <static_signed_type X>
    struct static_signed_sign : static_sign<static_signed_type, X>
    {
    };

    template <static_unsigned_type X>
    struct static_unsigned_sign
        : integral_constant<static_unsigned_type, X == 0 ? 0 : 1>
    {
    };

}
}


#endif
