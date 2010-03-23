/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_NUMERIC_HPP_JOFA_100322
#define BOOST_ITL_TYPE_TRAITS_IS_NUMERIC_HPP_JOFA_100322

#include <string>
#include <complex>

namespace boost{ namespace itl
{
    template <class Type> struct is_numeric
    {
        typedef is_numeric type;
        BOOST_STATIC_CONSTANT(bool, value = (0 < std::numeric_limits<Type>::digits));
    };

    template <class Type> 
    struct is_numeric<std::complex<Type> >
    {
        typedef is_numeric type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

}} // namespace boost itl

#endif


