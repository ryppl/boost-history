/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_interval_shared_hpp_JOFA_100306__
#define __test_itl_interval_shared_hpp_JOFA_100306__

#include <boost/itl/interval.hpp>
#include <boost/itl/interval_set.hpp>


template <class DomainT, ITL_COMPARE Compare, 
          template<class,ITL_COMPARE>class Interval>
void test_inner_complement(const Interval<DomainT,Compare>& itv1,
                           const Interval<DomainT,Compare>& itv2)
{
    typedef interval_set<DomainT,Compare,Interval> ItvSetT;
    typedef Interval<DomainT,Compare> IntervalT;

    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv1,itv2)), itl::distance(itv1,itv2));
    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv1,itv2)), itl::distance(itv2,itv1));
    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv2,itv1)), itl::distance(itv1,itv2));
    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv2,itv1)), itl::distance(itv2,itv1));

    ItvSetT itvset, inner_comp;
    itvset.add(itv1).add(itv2);
    ItvSetT hullset = ItvSetT(hull(itvset));
    inner_comp = hullset - itvset;
    IntervalT inner_comp_itv;
    if(inner_comp.begin() != inner_comp.end())
        inner_comp_itv = *inner_comp.begin();

    BOOST_CHECK_EQUAL(inner_complement(itv1,itv2), inner_comp_itv);
    BOOST_CHECK_EQUAL(inner_complement(itv2,itv1), inner_comp_itv);
    BOOST_CHECK_EQUAL(inner_comp.length(), itl::distance(itv1,itv2));
    BOOST_CHECK_EQUAL(inner_comp.length(), itl::distance(itv2,itv1));
}


#endif // __test_itl_interval_shared_hpp_JOFA_100306__
