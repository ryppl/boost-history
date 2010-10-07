/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_INTERVAL_HPP_JOFA_100327
#define BOOST_ITL_TYPE_TRAITS_IS_INTERVAL_HPP_JOFA_100327

#include <boost/itl/interval_bounds.hpp>
#include <boost/itl/interval_traits.hpp>

namespace boost{ namespace itl
{

template <class Type>
struct interval_bound_type
{
    typedef interval_bound_type type;
    BOOST_STATIC_CONSTANT(unsigned char, value = (interval_bounds::undefined)); 
};

template <class Type> struct is_interval
{ 
    typedef is_interval<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value < interval_bounds::undefined)); 
};

//JODO separate. Consider introducing interval_trais.
template <class Type> struct has_static_bounds
{ 
    typedef has_static_bounds<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value < interval_bounds::dynamic)); 
};

template <class Type> struct has_dynamic_bounds //JODO rearrange for all those predicates.
{ 
    typedef has_dynamic_bounds<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value == interval_bounds::dynamic));
};

template <class Type> struct has_asymmetric_bounds
{ 
    typedef has_asymmetric_bounds<Type> type;
    BOOST_STATIC_CONSTANT(unsigned char, bounds = (interval_bound_type<Type>::value)); 
    BOOST_STATIC_CONSTANT(bool, 
        value = (   bounds == interval_bounds::static_leftopen 
                 || bounds == interval_bounds::static_rightopen)); 
};

template <class Type> struct has_symmetric_bounds
{ 
    typedef has_symmetric_bounds<Type> type;
    BOOST_STATIC_CONSTANT(unsigned char, bounds = (interval_bound_type<Type>::value)); 
    BOOST_STATIC_CONSTANT(bool, 
        value = (   bounds == interval_bounds::static_closed 
                 || bounds == interval_bounds::static_open)); 
};

//------------------------------------------------------------------------------
template <class Type> struct is_discrete_static
{ 
    typedef is_discrete_static type;
    typedef typename interval_traits<Type>::domain_type domain_type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (mpl::and_< has_static_bounds<Type>
                          , is_discrete<domain_type> >::value) ); 
};

//------------------------------------------------------------------------------
template <class Type> struct is_static_rightopen
{ 
    typedef is_static_rightopen<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value == interval_bounds::static_rightopen)); 
};

template <class Type> struct is_static_leftopen
{ 
    typedef is_static_leftopen<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value == interval_bounds::static_leftopen)); 
};

template <class Type> struct is_static_open
{ 
    typedef is_static_open<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value == interval_bounds::static_open)); 
};

template <class Type> struct is_static_closed
{ 
    typedef is_static_closed<Type> type;
    BOOST_STATIC_CONSTANT(bool, 
        value = (interval_bound_type<Type>::value == interval_bounds::static_closed)); 
};

template <class Type> struct is_discrete_static_closed
{ 
    typedef is_static_closed<Type> type;
    typedef typename interval_traits<Type>::domain_type domain_type;
    
    BOOST_STATIC_CONSTANT( bool, 
        value = (mpl::and_< is_static_closed<Type>
                          , is_discrete<domain_type> >::value) ); 
};

template <class Type> struct is_discrete_static_open
{ 
    typedef is_static_closed<Type> type;
    typedef typename interval_traits<Type>::domain_type domain_type;
    
    BOOST_STATIC_CONSTANT( bool, 
        value = (mpl::and_< is_static_open<Type>
                          , is_discrete<domain_type> >::value) ); 
};

template <class Type> struct is_singelizable
{
    typedef is_singelizable type;
    typedef typename interval_traits<Type>::domain_type domain_type;

    BOOST_STATIC_CONSTANT(bool, 
        value =
            (mpl::or_< has_dynamic_bounds<Type>
                     , is_discrete<domain_type> 
                     >::value)
    );
};

}} // namespace boost itl

#endif


