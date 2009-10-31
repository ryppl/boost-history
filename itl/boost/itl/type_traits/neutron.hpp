/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_NEUTRON_HPP_JOFA_080912
#define BOOST_ITL_TYPE_TRAITS_NEUTRON_HPP_JOFA_080912

#include <boost/itl/type_traits/type_to_string.hpp>

namespace boost{ namespace itl
{
    template <class Type> struct neutron
    { 
        static Type value(); 
        Type operator()()const { return value(); }
    };

    template <class Type>
    inline Type neutron<Type>::value()
    { 
        return Type(); 
    }

    template<>
    inline std::string unary_template_to_string<neutron>::apply() { return "0"; }

}} // namespace boost itl

#endif


