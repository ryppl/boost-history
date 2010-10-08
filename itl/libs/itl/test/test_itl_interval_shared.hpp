/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_interval_shared_hpp_JOFA_100306__
#define __test_itl_interval_shared_hpp_JOFA_100306__

#include <boost/itl/interval_set.hpp>

/*CL after check
template <class T, ITL_COMPARE Compare, 
          ITL_INTERVAL(ITL_COMPARE)  Interval>
void interval_ctor_4_ordered_types()
{
    typedef Interval<T,Compare> IntervalT;
    // An empty interval is defined as the closed interval [1,0]
    BOOST_CHECK_EQUAL(itl::is_empty(IntervalT()), true);
    BOOST_CHECK_EQUAL(cardinality(IntervalT()), itl::identity_element<typename itl::size_type_of<T>::type>::value());
    BOOST_CHECK_EQUAL(itl::size(IntervalT()), itl::identity_element<typename itl::size_type_of<T>::type>::value());
    BOOST_CHECK_EQUAL(IntervalT().lower(), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(IntervalT().upper(), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT());
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT(itl::identity_element<T>::value(), itl::identity_element<T>::value()));
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT(itl::identity_element<T>::value(), itl::identity_element<T>::value(), interval_bounds::right_open()));
}


template <class T, ITL_COMPARE Compare, 
          ITL_INTERVAL(ITL_COMPARE)  Interval>
void interval_ctor_4_bicremental_types()
{
    typedef Interval<T,Compare> IntervalT;

    BOOST_CHECK_EQUAL( T(), pred(succ(T())));
    BOOST_CHECK_EQUAL( itl::identity_element<T>::value(), pred(succ(itl::identity_element<T>::value()))       );
    BOOST_CHECK_EQUAL( itl::unit_element<T>::value(),    succ(itl::identity_element<T>::value())             );
    BOOST_CHECK_EQUAL( interval<T>().length(),   itl::identity_element<typename difference_type_of<T>::type>::value() );

    T v4 = make<T>(4);
    IntervalT I4_4I(v4);
    BOOST_CHECK_EQUAL( I4_4I.bounds() == interval_bounds::closed(),      true  );
    BOOST_CHECK_EQUAL( I4_4I.bounds() == interval_bounds::left_open(),   false );
    BOOST_CHECK_EQUAL( I4_4I.bounds() == interval_bounds::left_open(),   false );
    BOOST_CHECK_EQUAL( I4_4I.bounds() == interval_bounds::right_open(),  false );
    BOOST_CHECK_EQUAL( I4_4I.bounds() == interval_bounds::open(),        false );
    //JODO
    //BOOST_CHECK_EQUAL( I4_4I.is_left(closed_bounded),  true  );
    //BOOST_CHECK_EQUAL( I4_4I.is_right(closed_bounded), true  );
    //BOOST_CHECK_EQUAL( I4_4I.is_left(open_bounded),    false );
    //BOOST_CHECK_EQUAL( I4_4I.is_right(open_bounded),   false );

    BOOST_CHECK_EQUAL( I4_4I.lower(),             v4    );
    BOOST_CHECK_EQUAL( I4_4I.upper(),             v4    );

    BOOST_CHECK_EQUAL( itl::contains(I4_4I, v4),       true  );
    BOOST_CHECK_EQUAL( itl::contains(I4_4I, I4_4I),    true  );
    BOOST_CHECK_EQUAL( within(I4_4I, I4_4I),      true  );
    BOOST_CHECK_EQUAL( I4_4I,                     I4_4I );
    BOOST_CHECK_EQUAL( cardinality(I4_4I),        unit_element<typename interval<T>::size_type>::value() );
    BOOST_CHECK_EQUAL( itl::size(I4_4I),          unit_element<typename interval<T>::size_type>::value() );

    IntervalT j_4_4(I4_4I);
    BOOST_CHECK_EQUAL( I4_4I, j_4_4 );
    IntervalT k_4_4;
    k_4_4 = j_4_4;
    BOOST_CHECK_EQUAL( I4_4I, k_4_4 );

    //JODO
    //T v2 = make<T>(2);
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4),    interval<T>(v2, v4) );
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4),    interval<T>(v2, v4, closed_bounded) );
    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4), interval<T>(v2, v4, right_open) );
    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4),  interval<T>(v2, v4, left_open) );
    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4),      interval<T>(v2, v4, open_bounded) );

    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).lower(),               v2 );
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).upper(),               v4 );
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).boundtype(),           closed_bounded );
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).bounds() == interval_bounds::closed_bounded(),    true );
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).is_left(closed_bounded),    true );
    //BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).is_right(closed_bounded),   true );

    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).lower(),            v2 );
    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).upper(),            v4 );
    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).boundtype(),        right_open );
    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).bounds() == interval_bounds::right_open(),     true );
    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).is_left(closed_bounded), true );
    //BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).is_right(open_bounded),  true );

    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).lower(),             v2 );
    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).upper(),             v4 );
    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).boundtype(),         left_open );
    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).bounds() == interval_bounds::left_open(),       true );
    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).is_left(open_bounded),    true );
    //BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).is_right(closed_bounded), true );

    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).lower(),                 v2 );
    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).upper(),                 v4 );
    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).boundtype(),            open_bounded );
    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).bounds() == interval_bounds::open_bounded(),               true );
    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).is_left(open_bounded),        true );
    //BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).is_right(open_bounded),       true );    
}

//JODO
//template <class T, ITL_COMPARE Compare, 
//          ITL_INTERVAL(ITL_COMPARE)  Interval>
//void interval_equal_4_bicremental_continuous_types()
//{
//    T v3 = make<T>(3);
//    T v7 = make<T>(7);
//    BOOST_CHECK_EQUAL(interval<T>(), interval<T>(v7,v3));
//
//    //I: (I)nside  = closed bound
//    //O: (O)utside = open bound
//    interval<T> I3_7I = interval<T>::closed(v3,v7);
//    interval<T> I3_7D = interval<T>::rightopen(v3,v7);
//    interval<T> C3_7I = interval<T>::leftopen(v3,v7);
//    interval<T> C3_7D = interval<T>::open(v3,v7);
//
//    BOOST_CHECK_EQUAL( I3_7I ,  I3_7I  );    
//    BOOST_CHECK_EQUAL( I3_7I == I3_7D, false  );    
//    BOOST_CHECK_EQUAL( I3_7I == C3_7D, false  );    
//    BOOST_CHECK_EQUAL( I3_7I == C3_7D, false );    
//    BOOST_CHECK_EQUAL( I3_7I != I3_7D, true  );    
//    BOOST_CHECK_EQUAL( I3_7I != C3_7D, true  );    
//    BOOST_CHECK_EQUAL( I3_7I != C3_7D, true );    
//
//    BOOST_CHECK_EQUAL( I3_7D ,  I3_7D  );    
//    BOOST_CHECK_EQUAL( I3_7D == C3_7I, false  );    
//    BOOST_CHECK_EQUAL( I3_7D == C3_7D, false );    
//    BOOST_CHECK_EQUAL( I3_7D != C3_7I, true  );    
//    BOOST_CHECK_EQUAL( I3_7D != C3_7D, true );    
//
//    BOOST_CHECK_EQUAL( C3_7I ,  C3_7I  );    
//    BOOST_CHECK_EQUAL( C3_7I == C3_7D, false );    
//    BOOST_CHECK_EQUAL( C3_7I != C3_7D, true );    
//
//    BOOST_CHECK_EQUAL( C3_7D,   C3_7D  );    
//} 

*/


template <class DomainT, ITL_COMPARE Compare, 
          ITL_INTERVAL(ITL_COMPARE)  Interval>
void test_inner_complement(const ITL_INTERVAL_TYPE(Interval,DomainT,Compare)& itv1,
                           const ITL_INTERVAL_TYPE(Interval,DomainT,Compare)& itv2)
{
    typedef interval_set<DomainT,Compare,Interval> ItvSetT;
    typedef ITL_INTERVAL_TYPE(Interval,DomainT,Compare) IntervalT;

    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv1,itv2)), itl::distance(itv1,itv2));
    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv1,itv2)), itl::distance(itv2,itv1));
    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv2,itv1)), itl::distance(itv1,itv2));
    BOOST_CHECK_EQUAL(itl::length(inner_complement(itv2,itv1)), itl::distance(itv2,itv1));

    IntervalT in_comp = inner_complement(itv1,itv2);
    ItvSetT itvset, inner_comp;
    itvset.add(itv1).add(itv2);
    ItvSetT hullset = ItvSetT(hull(itvset));
    inner_comp = hullset - itvset;
    IntervalT inner_comp_itv;
    if(inner_comp.begin() != inner_comp.end())
        inner_comp_itv = *inner_comp.begin();

    BOOST_CHECK_EQUAL(inner_complement(itv1,itv2), inner_comp_itv);
    BOOST_CHECK_EQUAL(inner_complement(itv2,itv1), inner_comp_itv);
    BOOST_CHECK_EQUAL(itl::length(inner_comp), itl::distance(itv1,itv2));
    BOOST_CHECK_EQUAL(itl::length(inner_comp), itl::distance(itv2,itv1));

    BOOST_CHECK(itl::disjoint(itv1, in_comp));
    BOOST_CHECK(itl::disjoint(itv2, in_comp));

    IntervalT itv1_comp = hull(itv1, in_comp);
    IntervalT itv2_comp = hull(itv2, in_comp);

    if(!itl::is_empty(in_comp))
    {
        BOOST_CHECK(itl::intersects(itv1_comp, in_comp));
        BOOST_CHECK(itl::intersects(itv2_comp, in_comp));

        BOOST_CHECK_EQUAL(itv1_comp & itv2_comp, in_comp);
        BOOST_CHECK_EQUAL( itl::is_empty(itv1_comp & itv2_comp), itl::disjoint(itv1_comp, itv2_comp));
        BOOST_CHECK_EQUAL(!itl::is_empty(itv1_comp & itv2_comp), itl::intersects(itv1_comp, itv2_comp));
    }
}

template <class IntervalT>
void test_inner_complement_(const IntervalT& itv1, const IntervalT& itv2)
{
    typedef typename interval_traits<IntervalT>::domain_type DomainT;
    // For the test of plain interval types we assume that std::less is
    // the compare functor
    test_inner_complement<DomainT, std::less, IntervalT>(itv1, itv2);
}



#endif // __test_itl_interval_shared_hpp_JOFA_100306__
