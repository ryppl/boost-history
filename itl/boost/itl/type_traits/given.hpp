/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_GIVEN_HPP_JOFA_100929
#define BOOST_ITL_TYPE_TRAITS_GIVEN_HPP_JOFA_100929

#include <boost/itl/type_traits/unit_element.hpp>

namespace boost{ namespace itl
{
    template<bool Fact, class Type = bool> struct given_;

    template<class Fact, class Type = bool> struct given
    {
        static Type then(const Type& success, const Type& fail = unit_element<Type>::value())
        {
            return given_<Fact::value, Type>::then(success, fail);
        }
    };

    //-----------------------------------------------------------------------------
    template<class Type> 
    struct given_<false, Type>
    {
        static Type then(const Type& success, const Type& fail = unit_element<Type>::value())     
        { return fail; }
    };

    template<class Type> 
    struct given_<true, Type> 
    {
        static Type then(const Type& success, const Type& fail = unit_element<Type>::value())     
        { return success; }
    };

}} // namespace boost itl

#endif


