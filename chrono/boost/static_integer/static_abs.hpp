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
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/static_integer/static_types.hpp>

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
#if BOOST_RATIO_WORK_WITH_MSVC    
    template <typename T, T X>
    struct static_abs 
        : integral_constant<T, 
            (X < 0) ? -X : X> 
    {};
#else
    template <typename T, T X>
    struct static_unary_minus;
        
    template <static_signed_type X>
    struct static_unary_minus<static_signed_type,X> : integral_constant<static_signed_type, -X> 
    {};

    namespace integer_detail
    {        
        template <typename T, T X, bool LT0>
        struct static_abs_aux2 : integral_constant<T, X> 
        {};
        template <typename T, T X>
        struct static_abs_aux2<T,X,true> : static_unary_minus<T, X> 
        {};
        template <typename T, T X, bool IsUnsigned>
        struct static_abs_aux : integral_constant<T, X> 
        {};
        template <typename T, T X>
        struct static_abs_aux<T,X,false> : static_abs_aux2<T, X, (X < 0) > 
        {};
    }
    
    template <typename T, T X>
    struct static_abs : integer_detail::static_abs_aux<T, X, is_unsigned<T>::value > 
    {};
        
#endif
        
    template <static_signed_type X>
    struct static_signed_abs 
        : static_abs<static_signed_type, X>
    {};

    template <static_signed_type X>
    struct static_unsigned_abs 
        : static_abs<static_unsigned_type, X> 
    {};


}  
}
#endif
