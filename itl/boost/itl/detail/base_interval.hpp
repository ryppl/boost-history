/*-----------------------------------------------------------------------------+
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_BASE_INTERVAL_HPP_JOFA_100323
#define BOOST_ITL_BASE_INTERVAL_HPP_JOFA_100323

#include <functional>
#include <boost/call_traits.hpp> 
#include <boost/concept_check.hpp> 
#include <boost/itl/detail/design_config.hpp>
#include <boost/itl/type_traits/difference_type_of.hpp>
#include <boost/itl/type_traits/size_type_of.hpp>

namespace boost{namespace itl
{

template <class DomainT, ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, DomainT)>
struct base_interval
{
    //==========================================================================
    //= Associated types
    //==========================================================================
    typedef base_interval<DomainT,Compare> type;

    //--------------------------------------------------------------------------
    //- Associated types: Data
    //--------------------------------------------------------------------------
    /// The domain type of the interval
    typedef DomainT domain_type;
    /// The codomaintype is the same as domain_type
    typedef DomainT codomain_type;
    /// The element type of the interval
    typedef DomainT element_type;
    /// The segment type is the interval's type
    typedef type    segment_type;
    /// The interval type is the interval's type
    typedef type    interval_type;

    //--------------------------------------------------------------------------
    //- Associated types: Implementation and stl related
    //--------------------------------------------------------------------------
    typedef DomainT key_type;
    typedef DomainT data_type;
    typedef DomainT value_type;

    /// Compare order on the data
    typedef ITL_COMPARE_DOMAIN(Compare,DomainT) domain_compare;
    typedef domain_compare key_compare;

    /// The difference type of an interval which is sometimes different form the domain_type
    typedef typename itl::difference_type_of<DomainT>::type difference_type;

    /// The size type of an interval which is mostly std::size_t
    typedef typename itl::size_type_of<DomainT>::type size_type;

    typedef typename boost::call_traits<DomainT>::param_type domain_param;

    /** Less compare of interval elements. */
    inline static bool domain_less(domain_param left, domain_param right)       
    { return domain_compare()(left, right) ;}

    /** Less or equal compare of interval elements. */
    inline static bool domain_less_equal(domain_param left, domain_param right) 
    { return !domain_compare()(right, left );}

    /** Equality compare of interval elements. */
    inline static bool domain_equal(domain_param left, domain_param right)
    { return !domain_compare()(left, right) && !domain_compare()(right, left);}
};

}} // namespace itl boost

#endif

