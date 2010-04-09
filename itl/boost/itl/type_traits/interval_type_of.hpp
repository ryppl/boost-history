/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_INTERVAL_TYPE_OF_HPP_JOFA_100403
#define BOOST_ITL_TYPE_TRAITS_INTERVAL_TYPE_OF_HPP_JOFA_100403

#include <boost/mpl/if.hpp>
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/continuous_interval.hpp>
#include <boost/itl/discrete_interval.hpp>
#include <boost/itl/type_traits/is_continuous.hpp>

namespace boost{ namespace itl
{
    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(DomainT,Compare)> 
    struct interval_type_of
    {
        typedef 
            typename mpl::if_< is_continuous<DomainT> //JODO add Traits to choose between static and dynamic intervals
                             , continuous_interval<DomainT,Compare>
                             ,   discrete_interval<DomainT,Compare> >::type type;
    };

}} // namespace boost itl

#endif


