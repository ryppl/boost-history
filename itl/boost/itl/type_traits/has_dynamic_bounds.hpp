/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_HAS_DYNAMIC_BOUNDS_HPP_JOFA_100327
#define BOOST_ITL_TYPE_TRAITS_HAS_DYNAMIC_BOUNDS_HPP_JOFA_100327

#include <boost/itl/interval_bounds.hpp>
#include <boost/itl/type_traits/is_interval.hpp>

namespace boost{ namespace itl
{

template <class Type> struct has_dynamic_bounds
{ 
    typedef has_dynamic_bounds<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
		value = (interval_bound_type<Type>::value == interval_bounds::dynamic));
};

template <class Type> struct is_dynamic_bounded //JODO rearrange for all those predicates.
{
    typedef is_dynamic_bounded<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (mpl::and_<is_interval<Type>, has_dynamic_bounds<Type> >::value)
        ); 
};

}} // namespace boost itl

#endif


