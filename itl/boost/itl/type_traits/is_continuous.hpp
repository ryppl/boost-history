/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_is_continuous_JOFA_080910_H__
#define __itl_type_traits_is_continuous_JOFA_080910_H__

#include <string>

namespace boost{ namespace itl
{
    template <class Type> struct is_continuous;

    template<> struct is_continuous<float>       { enum {value = true}; };
    template<> struct is_continuous<double>      { enum {value = true}; };
    template<> struct is_continuous<std::string> { enum {value = true}; };

#ifdef ITL_NEEDS_RATIONAL_IS_CONTINUOUS
#define ITL_HAS_RATIONAL_IS_CONTINUOUS
    template<class Integral> 
    struct is_continuous<boost::rational<Integral> > { enum {value = true}; };
#endif

    template <class Type> struct is_continuous{ enum {value = false}; };

}} // namespace boost itl

#endif


