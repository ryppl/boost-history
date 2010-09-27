/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_CONTINUOUS_HPP_JOFA_080910
#define BOOST_ITL_TYPE_TRAITS_IS_CONTINUOUS_HPP_JOFA_080910

#include <string>
#include <boost/mpl/not.hpp>
#include <boost/itl/type_traits/is_discrete.hpp>

namespace boost{ namespace itl
{
	//JODO Dies waere eigentlich besser, funktioniert aber nicht
    //template <class Type> struct is_continuous
    //{
    //    typedef is_continuous type;
    //    BOOST_STATIC_CONSTANT(bool, 
    //        value = mpl::not_<is_discrete<Type> >::value);
    //};

    template <class Type> struct is_continuous;

    template<> struct is_continuous<float>
    {
        typedef is_continuous<float> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> struct is_continuous<double>
    {
        typedef is_continuous<double> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> struct is_continuous<long double>
    {
        typedef is_continuous<long double> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template<> struct is_continuous<std::string>
    {
        typedef is_continuous<std::string> type;
        BOOST_STATIC_CONSTANT(bool, value = true);
    };

    template <class Type> struct is_continuous
    {
        typedef is_continuous<Type> type;
        BOOST_STATIC_CONSTANT(bool, value = false);
    };

}} // namespace boost itl

#endif


