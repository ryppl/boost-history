/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __test_itl_interval_map_shared_h_JOFA_081005__
#define __test_itl_interval_map_shared_h_JOFA_081005__

#include <boost/type_traits/is_same.hpp>

template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_fundamentals_4_ordered_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    typedef typename IntervalMapT::size_type       size_T;
    typedef typename IntervalMapT::difference_type diff_T;

    // ordered types is the largest set of instance types.
    // Because we can not generate values via incrementation for e.g. string,
    // we are able to test operations only for the most basic values
    // neutron (0, empty, T() ...) and unon.

    //T v0 = neutron<T>(); //JODO check operator() variant
    //T v1 = unon<T>();
    T v0 = neutron<T>::value();
    T v1 = unon<T>::value();
    interval<T> I0_0I(v0);
    interval<T> I1_1I(v1);
    interval<T> I0_1I(v0,v1);
    U u1 = unon<U>::value();

    //-------------------------------------------------------------------------
    //empty set
    //-------------------------------------------------------------------------
    BOOST_CHECK_EQUAL(IntervalMapT().empty(), true);
    BOOST_CHECK_EQUAL(IntervalMapT().cardinality(), neutron<size_T>::value());
    BOOST_CHECK_EQUAL(IntervalMapT().size(), neutron<size_T>::value());
    BOOST_CHECK_EQUAL(IntervalMapT().interval_count(), 0);
    BOOST_CHECK_EQUAL(IntervalMapT().iterative_size(), 0);
    BOOST_CHECK_EQUAL(IntervalMapT(), IntervalMapT());

    interval<T> mt_interval = neutron<interval<T> >::value();
    BOOST_CHECK_EQUAL(mt_interval, interval<T>());
    typename IntervalMapT::value_type mt_u1 = make_pair(mt_interval, u1);
    IntervalMapT mt_map = neutron<IntervalMapT >::value();
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    //adding emptieness to emptieness yields emptieness ;)
    mt_map.add(mt_u1).add(mt_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    mt_map.insert(mt_u1).insert(mt_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    (mt_map += mt_u1) += mt_u1;
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    BOOST_CHECK_EQUAL(enclosure(mt_map), neutron<interval<T> >::value());

    //subtracting emptieness
    mt_map.subtract(mt_u1).subtract(mt_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    mt_map.erase(mt_interval).erase(mt_interval);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    (mt_map -= mt_u1) -= mt_u1;
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    //subtracting elements form emptieness
    typename IntervalMapT::base_pair_type v0_u1 = make_pair(v0,u1);
    typename IntervalMapT::base_pair_type v1_u1 = make_pair(v1,u1);
    //mt_map.subtract(make_pair(v0,u1)).subtract(make_pair(v1,u1));
    mt_map.subtract(v0_u1).subtract(v1_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    mt_map.erase(v0_u1).erase(v1_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    (mt_map -= v0_u1) -= v1_u1;
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    //subtracting intervals form emptieness
    typename IntervalMapT::value_type I0_0I_u1 = make_pair(I0_0I,u1);
    typename IntervalMapT::value_type I0_1I_u1 = make_pair(I0_1I,u1);
    typename IntervalMapT::value_type I1_1I_u1 = make_pair(I1_1I,u1);
    mt_map.subtract(I0_1I_u1).subtract(I1_1I_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    mt_map.erase(I0_1I_u1).erase(I1_1I_u1);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    (mt_map -= I0_1I_u1) -= I1_1I_u1;
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    mt_map.erase(I0_1I).erase(I1_1I);
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    //insecting emptieness
    (mt_map *= mt_u1) *= mt_u1;
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());
    (mt_map *= mt_interval) *= mt_interval;
    BOOST_CHECK_EQUAL(mt_map, IntervalMapT());

    

    //-------------------------------------------------------------------------
    //unary set
    //-------------------------------------------------------------------------
    IntervalMapT single_I0_0I_u1_from_element(v0_u1);
    IntervalMapT single_I0_0I_u1_from_interval(I0_0I_u1);
    IntervalMapT single_I0_0I_u1(single_I0_0I_u1_from_interval);

    BOOST_CHECK_EQUAL(single_I0_0I_u1_from_element, single_I0_0I_u1_from_interval);
    BOOST_CHECK_EQUAL(single_I0_0I_u1_from_element, single_I0_0I_u1);
    BOOST_CHECK_EQUAL(enclosure(single_I0_0I_u1), I0_0I);
    BOOST_CHECK_EQUAL(single_I0_0I_u1.lower(), I0_0I.lower());
    BOOST_CHECK_EQUAL(single_I0_0I_u1.upper(), I0_0I.upper());

    IntervalMapT single_I1_1I_u1_from_element(v1_u1);
    IntervalMapT single_I1_1I_u1_from_interval(I1_1I_u1);
    IntervalMapT single_I1_1I_u1(single_I1_1I_u1_from_interval);

    BOOST_CHECK_EQUAL(single_I1_1I_u1_from_element, single_I1_1I_u1_from_interval);
    BOOST_CHECK_EQUAL(single_I1_1I_u1_from_element, single_I1_1I_u1);

    IntervalMapT single_I0_1I_u1_from_interval(I0_1I_u1);
    IntervalMapT single_I0_1I_u1(single_I0_1I_u1_from_interval);

    BOOST_CHECK_EQUAL(single_I0_1I_u1_from_interval, single_I0_1I_u1);
    BOOST_CHECK_EQUAL(enclosure(single_I0_1I_u1), I0_1I);
    BOOST_CHECK_EQUAL(single_I0_1I_u1.lower(), I0_1I.lower());
    BOOST_CHECK_EQUAL(single_I0_1I_u1.upper(), I0_1I.upper());

    //contains predicate
    BOOST_CHECK_EQUAL(single_I0_0I_u1.contains(v0), true);
    BOOST_CHECK_EQUAL(single_I0_0I_u1.contains(v0_u1), true);
    BOOST_CHECK_EQUAL(single_I0_0I_u1.contains(I0_0I_u1), true);

    BOOST_CHECK_EQUAL(single_I1_1I_u1.contains(v1), true);
    BOOST_CHECK_EQUAL(single_I1_1I_u1.contains(v1_u1), true);
    BOOST_CHECK_EQUAL(single_I1_1I_u1.contains(I1_1I_u1), true);

    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(v0), true);
    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(I0_1I_u1), true);
    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(v1), true);
    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(I1_1I_u1), true);

    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(single_I0_0I_u1), true);
    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(single_I1_1I_u1), true);
    BOOST_CHECK_EQUAL(single_I0_1I_u1.contains(single_I0_1I_u1), true);

    BOOST_CHECK_EQUAL(single_I0_0I_u1.cardinality(), unon<size_T>::value());
    BOOST_CHECK_EQUAL(single_I0_0I_u1.size(), unon<size_T>::value());
    BOOST_CHECK_EQUAL(single_I0_0I_u1.interval_count(), 1);
    BOOST_CHECK_EQUAL(single_I0_0I_u1.iterative_size(), 1);

}

template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_ctor_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;

    T v4 = make<T>(4);
    U u2 = make<U>(2);
    interval<T> I4_4I(v4);
    typename IntervalMapT::base_pair_type v4_u2(v4,u2);
    typename IntervalMapT::value_type I4_4I_u2(I4_4I,u2);

    IntervalMapT _I4_4I_u2;
    BOOST_CHECK_EQUAL( _I4_4I_u2.empty(), true );
    IntervalMapT _I4_4I_u2_1;
    IntervalMapT _I4_4I_u2_2;
    IntervalMapT _I4_4I_u2_3;
    _I4_4I_u2   += v4_u2;
    _I4_4I_u2_1 += I4_4I_u2;
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_1 );
    _I4_4I_u2_2.add(v4_u2);
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_2 );
    _I4_4I_u2_3.add(I4_4I_u2);
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_3 );

    _I4_4I_u2.clear();
    _I4_4I_u2.add(I4_4I_u2).add(I4_4I_u2);
    IntervalMapT _I4_4I_u4(make_pair(I4_4I, make<U>(4)));
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u4 );

    _I4_4I_u2.clear();
    _I4_4I_u2.insert(I4_4I_u2).insert(I4_4I_u2);
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_1 );

    BOOST_CHECK_EQUAL( _I4_4I_u2.cardinality(),      unon<typename IntervalMapT::size_type>::value()  );
    BOOST_CHECK_EQUAL( _I4_4I_u2.size(),             unon<typename IntervalMapT::size_type>::value()  );
    BOOST_CHECK_EQUAL( _I4_4I_u2.interval_count(),   1  );
    BOOST_CHECK_EQUAL( _I4_4I_u2.iterative_size(),   1  );
    BOOST_CHECK_EQUAL( _I4_4I_u2.lower(),            v4 );
    BOOST_CHECK_EQUAL( _I4_4I_u2.upper(),            v4 );

    IntervalMapT _I4_4I_u2_copy(_I4_4I_u2);
    IntervalMapT _I4_4I_u2_assigned;
    _I4_4I_u2_assigned = _I4_4I_u2;
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_copy );
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_assigned );

    _I4_4I_u2_assigned.clear();
    BOOST_CHECK_EQUAL( true,   _I4_4I_u2_assigned.empty() );

    _I4_4I_u2_assigned.swap(_I4_4I_u2_copy);
    BOOST_CHECK_EQUAL( true,   _I4_4I_u2_copy.empty() );
    BOOST_CHECK_EQUAL( _I4_4I_u2, _I4_4I_u2_assigned );
}


template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_add_sub_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;

    T v0 = make<T>(0);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v9 = make<T>(9);
    U u1 = make<U>(1);
    interval<T> I5_6I(v5,v6);
    interval<T> I5_9I(v5,v9);
    interval<T> I0_9I = interval<T>::closed(v0, v9);
    typename IntervalMapT::base_pair_type v0_u1 = make_pair(v0, u1);
    typename IntervalMapT::base_pair_type v9_u1 = make_pair(v9, u1);
    typename IntervalMapT::value_type I5_6I_u1 = make_pair(I5_6I, u1);
    typename IntervalMapT::value_type I5_9I_u1 = make_pair(I5_9I, u1);
    typename IntervalMapT::value_type I0_9I_u1 = make_pair(interval<T>::closed(v0, v9), u1);

    BOOST_CHECK_EQUAL( IntervalMapT(I5_6I_u1).add(v0_u1).add(v9_u1), 
                       IntervalMapT().add(v9_u1).add(I5_6I_u1).add(v0_u1) );

    IntervalMapT map_A = IntervalMapT(I5_6I_u1).add(v0_u1).add(v9_u1);
    IntervalMapT map_B = IntervalMapT().insert(v9_u1).insert(I5_6I_u1).insert(v0_u1);
    BOOST_CHECK_EQUAL( map_A, map_B );
    BOOST_CHECK_EQUAL( enclosure(map_A), I0_9I );
    BOOST_CHECK_EQUAL( map_A.lower(), I0_9I.lower() );
    BOOST_CHECK_EQUAL( map_A.upper(), I0_9I.upper() );
    //JODO: LAW !a.empty() => enclosure(a).lower() == a.lower()
    //JODO: LAW !a.empty() => enclosure(a).upper() == a.upper()

    IntervalMapT map_A1 = map_A, map_B1 = map_B,
                 map_A2 = map_A, map_B2 = map_B;

    map_A1.subtract(I5_6I_u1).subtract(v9_u1);
    map_B1.erase(v9_u1).erase(I5_6I_u1);
    BOOST_CHECK_EQUAL( map_A1, map_B1 );

    map_B1 = map_B;
    map_B2.erase(v9).erase(I5_6I);
    BOOST_CHECK_EQUAL( map_A1, map_B2 );

    map_A2.subtract(I5_9I_u1);
    map_B2.erase(I5_9I);
    BOOST_CHECK_EQUAL( map_A2, map_B2 );
}

template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_distinct_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    typedef typename IntervalMap<T,U>::size_type       size_T;
    typedef typename IntervalMap<T,U>::difference_type diff_T;
    T v1 = make<T>(1);
    T v3 = make<T>(3);
    T v5 = make<T>(5);
    U u1 = make<U>(1);
    typename IntervalMapT::base_pair_type v1_u1(v1,u1);
    typename IntervalMapT::base_pair_type v3_u1(v3,u1);
    typename IntervalMapT::base_pair_type v5_u1(v5,u1);

    size_T s3 = make<size_T>(3);

    IntervalMapT is_1_3_5;
    is_1_3_5.add(v1_u1).add(v3_u1).add(v5_u1);

    BOOST_CHECK_EQUAL( is_1_3_5.cardinality(),      s3 );
    BOOST_CHECK_EQUAL( is_1_3_5.size(),             s3 );
    BOOST_CHECK_EQUAL( is_1_3_5.interval_count(),   3 );
    BOOST_CHECK_EQUAL( is_1_3_5.iterative_size(),   3 );
}

template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_distinct_4_bicremental_continuous_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    typedef typename IntervalMapT::size_type       size_T;
    typedef typename IntervalMapT::difference_type diff_T;
    T v1 = make<T>(1);
    T v3 = make<T>(3);
    T v5 = make<T>(5);
    U u1 = make<U>(1);
    typename IntervalMapT::base_pair_type v1_u1(v1,u1);
    typename IntervalMapT::base_pair_type v3_u1(v3,u1);
    typename IntervalMapT::base_pair_type v5_u1(v5,u1);

    size_T s3 = make<size_T>(3);
    diff_T d0 = make<diff_T>(0);
    diff_T d2 = make<diff_T>(2);

    IntervalMapT is_1_3_5;
    is_1_3_5.add(v1_u1).add(v3_u1).add(v5_u1);

    BOOST_CHECK_EQUAL( is_1_3_5.cardinality(),      s3 );
    BOOST_CHECK_EQUAL( is_1_3_5.size(),             s3 );
    BOOST_CHECK_EQUAL( is_1_3_5.length(),           d0 );
    BOOST_CHECK_EQUAL( is_1_3_5.interval_count(),   3 );
    BOOST_CHECK_EQUAL( is_1_3_5.iterative_size(),   3 );


    IntervalMapT is_123_5;
    is_123_5 = is_1_3_5;
    //OPROM: open problem: Ambiguity resolving value_type and base_value_type for overloaded o= operators.
    //is_123_5 += make_pair(interval<T>::open(v1,v3),u1);                 //error C2593: 'operator +=' is ambiguous
    //is_123_5 += make_pair<interval<T>, U>(interval<T>::open(v1,v3),u1); //error C2593: 'operator +=' is ambiguous
    //USASO: unsatisfctory solution 1: explicit IntervalMapT::value_type instead of make_pair
    is_123_5 += typename IntervalMapT::value_type(interval<T>::open(v1,v3),u1);
    //USASO: unsatisfctory solution 2: not implementing base_value_type version of o=

    BOOST_CHECK_EQUAL( is_123_5.cardinality(),      std::numeric_limits<size_T>::infinity() );
    BOOST_CHECK_EQUAL( is_123_5.size(),             std::numeric_limits<size_T>::infinity() );
    BOOST_CHECK_EQUAL( is_123_5.length(),           d2 );

}

template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_isolate_4_bicremental_continuous_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    typedef typename IntervalMapT::size_type       size_T;
    typedef typename IntervalMapT::difference_type diff_T;

    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v4 = make<T>(4);
    U u1 = make<U>(1);
    interval<T> I0_4I = interval<T>::closed(v0,v4);
    interval<T> C0_2D = interval<T>::open(v0,v2);
    interval<T> C2_4D = interval<T>::open(v2,v4);
    typename IntervalMapT::value_type I0_4I_u1(I0_4I,u1);
    typename IntervalMapT::value_type C0_2D_u1(C0_2D,u1);
    typename IntervalMapT::value_type C2_4D_u1(C2_4D,u1);
    //   {[0               4]}
    // - {   (0,2)   (2,4)   }
    // = {[0]     [2]     [4]}
    IntervalMapT iso_map = IntervalMapT(I0_4I_u1);
    IntervalMapT gap_set;
    gap_set.add(C0_2D_u1).add(C2_4D_u1);
    iso_map -= gap_set;
    
    BOOST_CHECK_EQUAL( iso_map.cardinality(), static_cast<size_T>(3) );
    BOOST_CHECK_EQUAL( iso_map.iterative_size(), static_cast<std::size_t>(3) );

    IntervalMapT iso_map2;
    iso_map2.add(I0_4I_u1);
    iso_map2.subtract(C0_2D_u1).subtract(C2_4D_u1);
    
    IntervalMapT iso_map3(I0_4I_u1);
    (iso_map3 -= C0_2D_u1) -= C2_4D_u1;

    IntervalMapT iso_map4;
    iso_map4.insert(I0_4I_u1);
    iso_map4.erase(C0_2D_u1).erase(C2_4D_u1);
    
    BOOST_CHECK_EQUAL( iso_map, iso_map2 );
    BOOST_CHECK_EQUAL( iso_map, iso_map3 );
    BOOST_CHECK_EQUAL( iso_map, iso_map4 );
}


template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_contains_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    //LAW: x.add(e).contains(e); //false! 
    //LAW: x.insert(e).contains(e); //??
    T v3 = make<T>(3);
    T v5 = make<T>(5);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);
    T v11 = make<T>(11);
    U u1 = make<U>(1);
    
    typename IntervalMapT::base_pair_type v3_u1(v3,u1);
    typename IntervalMapT::base_pair_type v9_u1(v9,u1);
    typename IntervalMapT::base_pair_type v11_u1(v11,u1);

    typename IntervalMapT::value_type I3_7I_u1(interval<T>(v3,v7),u1);
    IntervalMapT im(v3_u1);    
    BOOST_CHECK_EQUAL( im.contains(v3_u1), true );

    BOOST_CHECK_EQUAL( IntervalMapT().add(v3_u1).contains(v3_u1), true );
    BOOST_CHECK_EQUAL( IntervalMapT().insert(v3_u1).contains(v3_u1), true );
    im.clear();
    BOOST_CHECK_EQUAL( (im += I3_7I_u1).contains(I3_7I_u1), true );

    IntervalMapT im0 = im;    

    im.clear();
    IntervalMapT im2(typename IntervalMapT::value_type(interval<T>::closed(v5,v8),u1));
    im2.add(v9_u1).add(v11_u1);
    im += im2;
    BOOST_CHECK_EQUAL( im.contains(im2), true );    
}

template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_operators_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v3 = make<T>(3);
    T v5 = make<T>(5);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    U u1 = make<U>(1);
    typename IntervalMapT::interval_type I3_5I(interval<T>::closed(v3,v5));
    typename IntervalMapT::value_type I0_1I_u1(interval<T>::closed(v0,v1),u1);
    typename IntervalMapT::value_type I3_5I_u1(interval<T>::closed(v3,v5),u1);
    typename IntervalMapT::value_type I7_8I_u1(interval<T>::closed(v7,v8),u1);
    
    IntervalMapT left, left2, right, all, section, complement;
    left.add(I0_1I_u1).add(I3_5I_u1);
    (right += I3_5I_u1) += I7_8I_u1;
    is_disjoint(left, right);
    BOOST_CHECK_EQUAL( is_disjoint(left, right), false );

    (all += left) += right;
    (section += left) *= right;
    all -= section;
    complement += all;
    //complement.erase(I3_5I);
    complement.erase(section);
    BOOST_CHECK_EQUAL( is_disjoint(section, complement), true );

    //JODO: There seems to be no intersection on maps of non set codomain type
    // that can be implemented via *= propagation. Intersectin on those types
    // could be dome elementic defining it via intersection on the set of pairs.
    //if(boost::is_same<T,int>::value)
    //{
    //    cout << "left:    " << left << endl;
    //    cout << "right:   " << right << endl;
    //    cout << "section: " << section << endl;
    //    cout << "complem: " << complement << endl;
    //    cout << "all:     " << all << endl;
    //    cout << "all2:    " << all2 << endl;
    //}

    //BOOST_CHECK_EQUAL( all.contains(left), true );
    //BOOST_CHECK_EQUAL( all.contains(right), true );
    //BOOST_CHECK_EQUAL( all.contains(complement), true );

    //BOOST_CHECK_EQUAL( left.contained_in(all), true );
    //BOOST_CHECK_EQUAL( right.contained_in(all), true );
    //BOOST_CHECK_EQUAL( complement.contained_in(all), true );
    //BOOST_CHECK_EQUAL( section.contained_in(left), true );
    //BOOST_CHECK_EQUAL( section.contained_in(right), true );
}


// Test for nontrivial intersection of interval maps with intervals and values
template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_base_intersect_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;

    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    U u1 = make<U>(1);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I1_4D = interval<T>::rightopen(v1,v4);
    interval<T> I1_8D = interval<T>::rightopen(v1,v8);
    interval<T> I2_7D = interval<T>::rightopen(v2,v7);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I5_8D = interval<T>::rightopen(v5,v8);
    interval<T> I6_7D = interval<T>::rightopen(v6,v7);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I6_9D = interval<T>::rightopen(v6,v9);

    typename IntervalMapT::value_type I0_3D_1(I0_3D, u1);
    typename IntervalMapT::value_type I6_9D_1(I6_9D, u1);
    typename IntervalMapT::value_type I1_3D_1(I1_3D, u1);
    typename IntervalMapT::value_type I6_8D_1(I6_8D, u1);
    typename IntervalMapT::value_type I2_3D_1(I2_3D, u1);
    typename IntervalMapT::value_type I6_7D_1(I6_7D, u1);

    //--------------------------------------------------------------------------
    //map_A        [0       3)       [6    9)
    //                     1           1
    //         *=      [1                8)
    //map_AB   ->      [1   3)       [6  8)
    //                     1           1
    //         *=        [2             7)     
    //         ->        [2 3)       [6 7)
    //                     1           1
    IntervalMap<T,U> map_A, map_AB, map_ab, map_ab2;
    interval_set<T>  set_B;
    map_A.add(I0_3D_1).add(I6_9D_1);
    map_AB = map_A;
    map_AB *= I1_8D;
    map_ab.add(I1_3D_1).add(I6_8D_1);

    BOOST_CHECK_EQUAL( map_AB, map_ab );

    map_AB = map_A;
    (map_AB *= I1_8D) *= I2_7D;
    map_ab2.add(I2_3D_1).add(I6_7D_1);

    BOOST_CHECK_EQUAL( map_AB, map_ab2 );

    //--------------------------------------------------------------------------
    //map_A        [0       3)       [6    9)
    //                     1           1
    //         *=      [1     4)  [5     8)
    //map_AB   ->      [1   3)       [6  8)
    //                     1           1
    //         *=        [2   4)  [5    7)     
    //         ->        [2 3)       [6 7)
    //                     1           1
    map_A.clear(); 
    map_A.add(I0_3D_1).add(I6_9D_1);
    set_B.add(I1_4D).add(I5_8D);
    map_AB = map_A;

    map_AB *= set_B;
    map_ab.clear();
    map_ab.add(I1_3D_1).add(I6_8D_1);
    BOOST_CHECK_EQUAL( map_AB, map_ab );

    //--------------------------------------------------------------------------
    //map_A      [0       3)       [6       9)
    //                1                1
    //         *=     1
    //map_AB ->      [1]
    //                1

    //JODO intersection with key-element not yet working
    //map_A.clear();
    //map_A.add(I0_3D_1).add(I6_9D_1);
    //map_AB = map_A;
    //map_AB *= v1;
    //map_ab.clear();
    //map_ab.add(v1);

    //BOOST_CHECK_EQUAL( map_AB, map_ab );

    //if(boost::is_same<T,int>::value)
    //{
    //    cout << "map_A : " << map_A << endl;
    //    cout << "map_AB: " << map_AB << endl;
    //}
}


// Test for nontrivial erasure of interval maps with intervals and interval sets
template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_base_erase_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;

    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    U u1 = make<U>(1);

    interval<T> I0_1D = interval<T>::rightopen(v0,v1);
    interval<T> I0_2D = interval<T>::rightopen(v0,v2);
    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I1_4D = interval<T>::rightopen(v1,v4);
    interval<T> I1_8D = interval<T>::rightopen(v1,v8);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);
    interval<T> I2_7D = interval<T>::rightopen(v2,v7);
    interval<T> I2_3D = interval<T>::rightopen(v2,v3);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I5_8D = interval<T>::rightopen(v5,v8);
    interval<T> I6_7D = interval<T>::rightopen(v6,v7);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I6_9D = interval<T>::rightopen(v6,v9);
    interval<T> I7_9D = interval<T>::rightopen(v7,v9);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);

    typename IntervalMapT::value_type I0_1D_1(I0_1D, u1);
    typename IntervalMapT::value_type I0_3D_1(I0_3D, u1);
    typename IntervalMapT::value_type I0_2D_1(I0_2D, u1);
    typename IntervalMapT::value_type I6_9D_1(I6_9D, u1);
    typename IntervalMapT::value_type I1_3D_1(I1_3D, u1);
    typename IntervalMapT::value_type I6_8D_1(I6_8D, u1);
    typename IntervalMapT::value_type I2_3D_1(I2_3D, u1);
    typename IntervalMapT::value_type I6_7D_1(I6_7D, u1);
    typename IntervalMapT::value_type I7_9D_1(I7_9D, u1);
    typename IntervalMapT::value_type I8_9D_1(I8_9D, u1);

    //--------------------------------------------------------------------------
    //map_A        [0        3)       [6       9)
    //                  1                  1
    //      erase        [2              7)
    //map_A2   ->  [0   2)                [7   9)
    //                 1                    1
    //      erase      [1                   8)     
    //         ->  [0 1)                    [8 9)
    //               1                       1
    IntervalMap<T,U> map_A, map_A2, map_A3, map_check2, map_check3;
    interval_set<T>  set_B;
    map_A.add(I0_3D_1).add(I6_9D_1);
    map_A2 = map_A;
    map_A2.erase(I2_7D);
    map_check2.add(I0_2D_1).add(I7_9D_1);
    BOOST_CHECK_EQUAL( map_A2, map_check2 );

    map_A3 = map_A2;
    map_A3.erase(I1_8D);
    map_check3.add(I0_1D_1).add(I8_9D_1);
    BOOST_CHECK_EQUAL( map_A3, map_check3 );

          
    //--------------------------------------------------------------------------
    //map_A        [0        3)       [6       9)
    //                  1                  1
    //      erase        [2              7)
    //         ->  [0   2)                [7   9)
    //                 1                    1
    //      erase      [1                   8)     
    //         ->  [0 1)                    [8 9)
    //               1                       1
    map_A3 = map_A;
    map_A3.erase(I2_7D).erase(I1_8D);
    BOOST_CHECK_EQUAL( map_A3, map_check3 );

    //--------------------------------------------------------------------------
    //map_A        [0        3)       [6       9)
    //                  1                  1
    //         -=        [2              7)
    //         ->  [0   2)                [7   9)
    //                 1                    1
    //         -=      [1                   8)     
    //         ->  [0 1)                    [8 9)
    //               1                       1
    map_A3 = map_A;
    (map_A3 -= I2_7D) -= I1_8D;
    BOOST_CHECK_EQUAL( map_A3, map_check3 );

    //--------------------------------------------------------------------------
    //map_A        [0        3)       [6       9)
    //                  1                  1
    //      erase        [2    4)   [5   7)
    //         ->  [0   2)                [7   9)
    //                 1                    1
    map_A3 = map_A;
    set_B.add(I2_4D).add(I5_7D);
    map_A3 -= set_B;
    BOOST_CHECK_EQUAL( map_A3, map_check2 );
}


// Test first_collision
template <template<class T, class U,
                   class Traits = neutron_absorber,
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_COMPARE Compare = std::less,
                   ITL_COMBINE Combine = ITL_INPLACE_PLUS(U),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap, 
          class T, class U>
void interval_map_base_is_disjoint_4_bicremental_types()
{
    typedef IntervalMap<T,U> IntervalMapT;
    typedef typename IntervalMap<T,U>::interval_set_type IntervalSetT;

    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v3 = make<T>(3);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    U u1 = make<U>(1);

    interval<T> I0_1D = interval<T>::rightopen(v0,v1);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);

    typename IntervalMapT::value_type I0_1D_1(I0_1D, u1);
    typename IntervalMapT::value_type I1_3D_1(I1_3D, u1);
    typename IntervalMapT::value_type I3_6D_1(I3_6D, u1);
    typename IntervalMapT::value_type I5_7D_1(I5_7D, u1);
    typename IntervalMapT::value_type I6_8D_1(I6_8D, u1);
    typename IntervalMapT::value_type I8_9D_1(I8_9D, u1);

    //--------------------------------------------------------------------------
    //map_A          [1      3)       [6       8)
    //                  1                  1
    //map_B      [0 1)        [3     6)         [8 9)
    //             1              1               1
    IntervalMapT map_A, map_B;
    IntervalSetT set_A, set_B;

    map_A.add(I1_3D_1).add(I6_8D_1);
    map_B.add(I0_1D_1).add(I3_6D_1).add(I8_9D_1);
    BOOST_CHECK_EQUAL( is_disjoint(map_A, map_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(map_B, map_A), true );

    map_A.domain(set_A);
    map_B.domain(set_B);
    BOOST_CHECK_EQUAL( is_disjoint(map_A, set_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(set_B, map_A), true );
    BOOST_CHECK_EQUAL( is_disjoint(set_A, map_B), true );
    BOOST_CHECK_EQUAL( is_disjoint(map_B, set_A), true );

    map_A += I5_7D_1;

    BOOST_CHECK_EQUAL( is_disjoint(map_A, map_B), false );
    BOOST_CHECK_EQUAL( is_disjoint(map_B, map_A), false );

    map_A.domain(set_A);
    map_B.domain(set_B);
    BOOST_CHECK_EQUAL( is_disjoint(map_A, set_B), false );
    BOOST_CHECK_EQUAL( is_disjoint(set_B, map_A), false );
    BOOST_CHECK_EQUAL( is_disjoint(set_A, map_B), false );
    BOOST_CHECK_EQUAL( is_disjoint(map_B, set_A), false );
}


#endif // __test_itl_interval_map_shared_h_JOFA_080920__

