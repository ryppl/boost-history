/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_INTERVAL_TYPE_DEFAULT_HPP_JOFA_100403
#define BOOST_ITL_TYPE_TRAITS_INTERVAL_TYPE_DEFAULT_HPP_JOFA_100403

#include <boost/mpl/if.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/continuous_interval.hpp>
#include <boost/itl/discrete_interval.hpp>
#include <boost/itl/rightopen_interval.hpp>
#include <boost/itl/leftopen_interval.hpp>
#include <boost/itl/closed_interval.hpp>
#include <boost/itl/open_interval.hpp>
#include <boost/itl/type_traits/is_continuous.hpp>
#include <boost/itl/type_traits/is_discrete.hpp>

namespace boost{ namespace itl
{
    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)> 
    struct interval_type_default
    {
#ifdef ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS
        typedef
            typename mpl::if_< is_discrete<DomainT>
                             , right_open_interval<DomainT,Compare>
                             , right_open_interval<DomainT,Compare> >::type type;
#else
        typedef
            typename mpl::if_< is_discrete<DomainT>
                             ,   discrete_interval<DomainT,Compare>
                             , continuous_interval<DomainT,Compare> >::type type;
#endif
    };

}} // namespace boost itl

#endif


