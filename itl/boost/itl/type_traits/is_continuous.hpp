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
#include <boost/mpl/and.hpp>

namespace boost{ namespace itl
{
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
            //JODO Meta Fortsetzung des Praedikats auf interval<Type> etc.
            //  so nicht, vielleicht anders? Meta lambda?
            //value = (mpl::and_<has_domain<Type>::value, 
            //                   is_continuous<typename Type::domain_type>
            //                  >::value
            //         ));
    };

}} // namespace boost itl

#endif


