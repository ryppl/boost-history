/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_ASYMMETRIC_INTERVAL_HPP_JOFA_100327
#define BOOST_ITL_TYPE_TRAITS_IS_ASYMMETRIC_INTERVAL_HPP_JOFA_100327

#include <boost/itl/type_traits/is_interval.hpp>

namespace boost{ namespace itl
{

template <class Type> struct is_asymmetric_interval
{ 
    typedef is_asymmetric_interval<Type> type;
    BOOST_STATIC_CONSTANT(bool,
            value = (type_traits::ice_and
                    <
                        is_interval<Type>::value
                    ,   has_static_bounds<Type>::value
                    ,   has_asymmetric_bounds<Type>::value
                    >::value)
                );
};

template <class Type> struct is_continuous_asymmetric
{ 
    typedef is_continuous_asymmetric<Type> type;
    BOOST_STATIC_CONSTANT(bool,
            value = (type_traits::ice_and
                    <
                        is_asymmetric_interval<Type>::value
					,   is_continuous<typename Type::domain_type>::value
                    >::value)
                );
};

template <class Type> struct is_discrete_asymmetric
{ 
    typedef is_discrete_asymmetric<Type> type;
    BOOST_STATIC_CONSTANT(bool,
            value = (type_traits::ice_and
                    <
                        is_asymmetric_interval<Type>::value
					,   type_traits::ice_not<is_continuous<typename Type::domain_type>::value>::value
                    >::value)
                );
};

}} // namespace boost itl

#endif


