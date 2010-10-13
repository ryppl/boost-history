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
#include <boost/itl/right_open_interval.hpp>
#include <boost/itl/left_open_interval.hpp>
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
#  ifdef BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT 
                             , BOOST_ICL_DISCRETE_STATIC_INTERVAL_DEFAULT<DomainT,Compare>
#  else
                             , right_open_interval<DomainT,Compare>
#  endif

#  ifdef BOOST_ICL_CONTINUOUS_STATIC_INTERVAL_DEFAULT 
                             , BOOST_ICL_CONTINUOUS_STATIC_INTERVAL_DEFAULT<DomainT,Compare> 
#  else
                             , right_open_interval<DomainT,Compare>
#  endif
                             >::type type;
#else
        typedef
            typename mpl::if_< is_discrete<DomainT>
                             ,   discrete_interval<DomainT,Compare>
                             , continuous_interval<DomainT,Compare> >::type type;
#endif
    };


//==============================================================================
//JODO move?
    template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)> 
    struct _interval
    {
        typedef typename interval_type_default<DomainT>::type interval_type;
        typedef interval_type type;

#ifdef ITL_USE_STATIC_INTERVAL_BORDER_DEFAULTS

        static inline interval_type open(const DomainT& low, const DomainT& up)
        {
            return 
                static_interval
                < interval_type                // if the domain_type is discrete ...
                , is_discrete<typename interval_traits<interval_type>::domain_type>::value
                , interval_bounds::static_open // 'pretended' bounds will be transformed to
                , interval_bound_type<interval_type>::value // the represented bounds
                >
                ::construct(low, up); 
        }

        static inline interval_type left_open(const DomainT& low, const DomainT& up)
        {
            return 
                static_interval
                < interval_type
                , is_discrete<typename interval_traits<interval_type>::domain_type>::value
                , interval_bounds::static_left_open
                , interval_bound_type<interval_type>::value
                >
                ::construct(low, up); 
        }

        static inline interval_type right_open(const DomainT& low, const DomainT& up)
        {
            return 
                static_interval
                < interval_type
                , is_discrete<typename interval_traits<interval_type>::domain_type>::value
                , interval_bounds::static_right_open
                , interval_bound_type<interval_type>::value
                >
                ::construct(low, up); 
        }

        static inline interval_type closed(const DomainT& low, const DomainT& up)
        {
            return 
                static_interval
                < interval_type
                , is_discrete<typename interval_traits<interval_type>::domain_type>::value
                , interval_bounds::static_closed
                , interval_bound_type<interval_type>::value
                >
                ::construct(low, up); 
        }

        static inline interval_type construct(const DomainT& low, const DomainT& up)
        { return itl::construct<interval_type>(low, up); }

#else // ITL_USE_DYNAMIC_INTERVAL_BORDER_DEFAULTS
        static inline interval_type right_open(const DomainT& low, const DomainT& up)
        { return itl::construct<interval_type>(low, up, interval_bounds::right_open()); }

        static inline interval_type left_open(const DomainT& low, const DomainT& up)
        { return itl::construct<interval_type>(low, up, interval_bounds::left_open()); }

        static inline interval_type open(const DomainT& low, const DomainT& up)
        { return itl::construct<interval_type>(low, up, interval_bounds::open()); }

        static inline interval_type closed(const DomainT& low, const DomainT& up)
        { return itl::construct<interval_type>(low, up, interval_bounds::closed()); }

        static inline interval_type construct(const DomainT& low, const DomainT& up)
        { return itl::construct<interval_type>(low, up); }

#endif 
    };

    template <class IntervalT, bool IsDiscrete, bound_type PretendedBounds, bound_type RepresentedBounds> 
    struct static_interval;

    template <class IntervalT, bound_type PretendedBounds, bound_type RepresentedBounds> 
    struct static_interval<IntervalT, true, PretendedBounds, RepresentedBounds>
    {// is_discrete<domain_type<IntervalT>>
        typedef typename interval_traits<IntervalT>::domain_type domain_type;

        static inline IntervalT construct(const domain_type& low, const domain_type& up)
        {
            return itl::construct<IntervalT>(
                  shift_lower(interval_bounds(PretendedBounds), interval_bounds(RepresentedBounds), low)
                , shift_upper(interval_bounds(PretendedBounds), interval_bounds(RepresentedBounds), up )
                ); 
        }
    };

    template <class IntervalT, bound_type PretendedBounds, bound_type RepresentedBounds> 
    struct static_interval<IntervalT, false, PretendedBounds, RepresentedBounds>
    {// !is_discrete<domain_type<IntervalT>>
        typedef typename interval_traits<IntervalT>::domain_type domain_type;

        static inline IntervalT construct(const domain_type& low, const domain_type& up)
        {
            BOOST_STATIC_ASSERT((is_discrete<domain_type>::value || PretendedBounds==RepresentedBounds));
            // For domain_types that are not discrete, e.g. interval<float> 
            // one of the following must hold: If you call
            // interval<T>::right_open(x,y) then interval<T>::type must be static_right_open
            // interval<T>::left_open(x,y)  then interval<T>::type must be static_left_open
            // interval<T>::open(x,y)       then interval<T>::type must be static_open
            // interval<T>::closed(x,y)     then interval<T>::type must be static_closed
            // Conversion between 'PretendedBounds' and 'RepresentedBounds' is only possible
            // for discrete domain_types.
            return itl::construct<IntervalT>(low, up);
        }
    };

}} // namespace boost itl

#endif


