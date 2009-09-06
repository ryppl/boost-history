/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __itl_type_traits_has_inverse_JOFA_090205_H__
#define __itl_type_traits_has_inverse_JOFA_090205_H__

#include <boost/type_traits/is_signed.hpp>

namespace boost{ namespace itl
{

    template <class Type> struct has_inverse
    { 
        typedef has_inverse<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = (is_signed<Type>::value)); 
    };

}} // namespace boost itl

#endif // __itl_type_traits_has_inverse_JOFA_090205_H__


