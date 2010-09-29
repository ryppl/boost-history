/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_GIVEN_HPP_JOFA_100929
#define BOOST_ITL_TYPE_TRAITS_GIVEN_HPP_JOFA_100929

namespace boost{ namespace itl
{
    template<bool fact> struct given;
    template<> struct given<false>{static bool then(bool)     { return true;}};
    template<> struct given<true> {static bool then(bool fact){ return fact;}};

}} // namespace boost itl

#endif


