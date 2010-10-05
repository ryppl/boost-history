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
                             , closed_interval<DomainT,Compare>
                             , rightopen_interval<DomainT,Compare> >::type type;
#else
        typedef
            typename mpl::if_< is_discrete<DomainT>
                             ,   discrete_interval<DomainT,Compare>
                             , continuous_interval<DomainT,Compare> >::type type;
#endif
    };


    //-----------------------------------------------------------------------------
    //-----------------------------------------------------------------------------
/*JODO
    template<class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
    struct use_static_interval_borders
    {
        typedef use_static_interval_borders type;
        BOOST_STATIC_CONSTANT(bool, value = false);

    }

    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)
              bool StaticBorders = use_static_interval_borders<DomainT, Compare>,
              bool HasContinuousDomain = is_continuous<DomainT> > 
    struct interval_type_default;


    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)
              bool HasContinuousDomain = is_continuous<DomainT>     > 
    struct interval_type_default<DomainT, Compare, true, HasContinuousDomain>
    {
        typedef rightopen_interval<DomainT,Compare> type;
    };

    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT) >
    struct interval_type_default<DomainT, Compare, false, true>
    {
        typedef continuous_interval<DomainT,Compare> type;
    };

    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT) >
    struct interval_type_default<DomainT, Compare, false, false>
    {
        typedef discrete_interval<DomainT,Compare> type;
    };




    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)> 
    struct interval_type_default
    {
        typedef
            typename mpl::if_< use_static_interval_borders<DomainT>
                             , rightopen_interval<DomainT,Compare>
                             , typename mpl::if_< is_continuous<DomainT>
                                                , continuous_interval<DomainT,Compare>
                                                ,   discrete_interval<DomainT,Compare> >::type >::type type;
    };

*/

}} // namespace boost itl

#endif


