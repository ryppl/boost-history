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

namespace integer_detail
{
    template <typename T, bool X_is_0, T X, bool Y_is_0, T Y>
    struct static_gcd_aux 
        : static_gcd_aux<T, Y==0, Y, (X % Y)==0, X % Y>
    {};
    
    template <typename T, T X, T Y>
    struct static_gcd_aux<T, false, X, true, Y> : integral_constant<T, X> 
    {};

    template <typename T, T X, T Y, bool C>
    struct static_gcd_aux<T, true, X, C, Y> : integral_constant<T, Y> 
    {};
}    
    template <typename T, T X, T Y>
    struct static_gcd : integer_detail::static_gcd_aux<T, X==0, X, Y==0, Y>
    {};

    //~ template <typename T, T X, T Y>
    //~ struct static_gcd 
        //~ : static_gcd<T, Y, X % Y>
    //~ {};
    
    //~ template <typename T, T X>
    //~ struct static_gcd<T, X, T(0)> : integral_constant<T, X> 
    //~ {};

    //~ template <typename T, T X>
    //~ struct static_gcd<T, T(0), X> : integral_constant<T, X> 
    //~ {};
        
    typedef boost::intmax_t  static_gcd_signed_type;
    typedef boost::uintmax_t  static_gcd_unsigned_type;

   
    template <static_gcd_signed_type X, static_gcd_signed_type Y>
    struct static_signed_gcd : static_gcd<static_gcd_signed_type, X, Y>
    {};
    

    template <static_gcd_unsigned_type X, static_gcd_unsigned_type Y>
    struct static_unsigned_gcd : static_gcd<static_gcd_unsigned_type, X, Y>
    {};
    
}
}


#endif
