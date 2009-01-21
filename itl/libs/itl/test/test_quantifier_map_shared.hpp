/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#ifndef __test_itl_quantifier_map_shared_h_JOFA_090119__
#define __test_itl_quantifier_map_shared_h_JOFA_090119__


template <class T, class U,
          template<class T, class U,
                   class Traits = neutron_absorber,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_star, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap1, 
          template<class T, class U,
                   class Traits = neutron_absorber,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_star, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap2 
          >
void interval_map_base_laws_plus_4_bicremental_types()
{
	/*
    typedef IntervalMap1<T,U> IntervalMap1T;
    typedef IntervalMap2<T,U> IntervalMap2T;

    T v0 = make<T>(0);
    T v1 = make<T>(1);
    T v3 = make<T>(3);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    T v9 = make<T>(9);

    U u1 = make<U>(1);
    U u2 = make<U>(2);

	interval<T> I0_1D = interval<T>::rightopen(v0,v1);
    interval<T> I1_3D = interval<T>::rightopen(v1,v3);
    interval<T> I3_6D = interval<T>::rightopen(v3,v6);
    interval<T> I5_7D = interval<T>::rightopen(v5,v7);
    interval<T> I6_8D = interval<T>::rightopen(v6,v8);
    interval<T> I8_9D = interval<T>::rightopen(v8,v9);

    typename IntervalMap1T::value_type I0_1D_1(I0_1D, u1);
    typename IntervalMap1T::value_type I1_3D_1(I1_3D, u1);
    typename IntervalMap1T::value_type I3_6D_1(I3_6D, u1);
    typename IntervalMap1T::value_type I5_7D_1(I5_7D, u1);
    typename IntervalMap1T::value_type I6_8D_1(I6_8D, u1);
    typename IntervalMap1T::value_type I8_9D_1(I8_9D, u1);

	IntervalMap1T map_a, map_b;
	map_a.add(I3_6D_1).add(I5_7D_1);
	map_b.add(I1_3D_1).add(I8_9D_1);
	map_a = map_a;
	typename IntervalMap1T::value_type val_pair = I8_9D_1;

	IntervalMap2T map2_a, map2_b;
	map2_a = map_a;
	map2_a += map_a;
	IntervalMap2T join_map = map_a + map2_a;
	//IntervalMap2T splt_map = map_a + map2_a;

	check_commutativity_wrt_plus(map_a, map_b);
	//check_commutativity_plus(map2_a, map_b);
	check_commutativity_wrt_plus(map_a, val_pair);
	typename IntervalMap1T::domain_mapping_type v5_u2(v5,u2);
	check_commutativity_wrt_plus(map_b, v5_u2);

	CHECK_ASSOCIATIVITY_WRT(plus)(map_a, map_b, map_a);
	check_associativity_wrt_plus(map_a, map_b, map_a);

	check_neutrality_wrt_plus(map_a, neutron<IntervalMap1T>::value());

	CHECK_MONOID_WRT(plus)(neutron<IntervalMap1T>::value(), map_a, map_b, map_a);
	*/
}


template <class T, class U, class JointMapT, class SplitMapT>
void quantifier_map_check_monoid_instance_plus
(
const JointMapT& join_map_a, const JointMapT& join_map_b, const JointMapT& join_map_c,
const SplitMapT& split_map_a, const SplitMapT& split_map_b, const SplitMapT& split_map_c,
const typename JointMapT::value_type& val_pair, const mapping_pair<T,U>& map_pair
)
{
	/*
	//-----------------------------------------------------------------------------
	CHECK_MONOID_WRT(plus)(neutron<JointMapT>::value(), join_map_a, join_map_b, val_pair);
	CHECK_MONOID_WRT(plus)(neutron<JointMapT>::value(), join_map_a, val_pair, join_map_b);

	CHECK_MONOID_WRT(plus)(neutron<JointMapT>::value(), join_map_c, join_map_b, map_pair);
	CHECK_MONOID_WRT(plus)(neutron<JointMapT>::value(), join_map_c, map_pair, join_map_b);

	//-----------------------------------------------------------------------------
	CHECK_MONOID_WRT(plus)(neutron<SplitMapT>::value(), split_map_a, split_map_b, val_pair);
	CHECK_MONOID_WRT(plus)(neutron<SplitMapT>::value(), split_map_a, val_pair, split_map_b);

	CHECK_MONOID_WRT(plus)(neutron<SplitMapT>::value(), split_map_c, split_map_b, map_pair);
	CHECK_MONOID_WRT(plus)(neutron<SplitMapT>::value(), split_map_c, map_pair, split_map_b);

	//-----------------------------------------------------------------------------
	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, join_map_b, split_map_c);
	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, split_map_b, join_map_c);

	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, join_map_b, join_map_c);
	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, join_map_b, join_map_c);

	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, join_map_b, map_pair);
	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, map_pair, join_map_b);

	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, join_map_b, val_pair);
	CHECK_MONOID_WRT_EQUAL(plus)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, val_pair, join_map_b);
	*/
}


template <class T, class U, class JointMapT, class SplitMapT>
void quantifier_map_check_monoid_instance_et
(
const JointMapT& join_map_a, const JointMapT& join_map_b, const JointMapT& join_map_c,
const SplitMapT& split_map_a, const SplitMapT& split_map_b, const SplitMapT& split_map_c,
const typename JointMapT::value_type& val_pair, const mapping_pair<T,U>& map_pair
)
{
	/*
	//-----------------------------------------------------------------------------
	CHECK_MONOID_WRT(et)(neutron<JointMapT>::value(), join_map_a, join_map_b, val_pair);
	CHECK_MONOID_WRT(et)(neutron<JointMapT>::value(), join_map_a, val_pair, join_map_b);

	CHECK_MONOID_WRT(et)(neutron<JointMapT>::value(), join_map_c, join_map_b, map_pair);
	CHECK_MONOID_WRT(et)(neutron<JointMapT>::value(), join_map_c, map_pair, join_map_b);

	//-----------------------------------------------------------------------------
	CHECK_MONOID_WRT(et)(neutron<SplitMapT>::value(), split_map_a, split_map_b, val_pair);
	CHECK_MONOID_WRT(et)(neutron<SplitMapT>::value(), split_map_a, val_pair, split_map_b);

	CHECK_MONOID_WRT(et)(neutron<SplitMapT>::value(), split_map_c, split_map_b, map_pair);
	CHECK_MONOID_WRT(et)(neutron<SplitMapT>::value(), split_map_c, map_pair, split_map_b);

	//-----------------------------------------------------------------------------
	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, join_map_b, split_map_c);
	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, split_map_b, join_map_c);

	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, join_map_b, join_map_c);
	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_a, join_map_b, join_map_c);

	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, join_map_b, map_pair);
	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, map_pair, join_map_b);

	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, join_map_b, val_pair);
	CHECK_MONOID_WRT_EQUAL(et)(is_element_equal, neutron<SplitMapT>::value(), split_map_c, val_pair, join_map_b);
	*/
}

template <class T, class U, class Trait>
void quantifier_map_check_monoid_4_bicremental_types()
{
    typedef interval_map<T,U,Trait> IntervalMapT;
    typedef split_interval_map<T,U,Trait> SplitIntervalMapT;

	IntervalMapT join_map_a, join_map_b, join_map_c;
	join_map_a.add(IDv(3,6,1)).add(IIv(5,7,1));
	join_map_b.add(CDv(1,3,1)).add(IDv(8,9,1));
	join_map_c.add(IDv(0,9,2)).add(IIv(3,6,1)).add(IDv(5,7,1));

	SplitIntervalMapT split_map_a, split_map_b, split_map_c;
	split_map_a.add(IDv(0,6,2)).add(IDv(5,7,1)).add(IDv(9,10,2));
	split_map_b.add(IDv(2,3,3)).add(IIv(9,9,3)).add(CDv(9,11,2));
	split_map_c.add(CIv(0,9,2)).add(IIv(3,6,1)).add(CDv(5,7,1));

	typename IntervalMapT::interval_mapping_type val_pair = IDv(6,9,1);
	mapping_pair<T,U> map_pair = K_v(5,1);

	IntervalMapT jlhs = join_map_a + join_map_b;
	IntervalMapT jrhs = join_map_b + join_map_a;
	BOOST_CHECK_EQUAL(jlhs, jrhs);

	jlhs = join_map_a + val_pair;
	jrhs = val_pair + join_map_a;
	BOOST_CHECK_EQUAL(jlhs, jrhs);

	SplitIntervalMapT slhs = split_map_a + split_map_b;
	SplitIntervalMapT srhs = split_map_b + split_map_a;
	BOOST_CHECK_EQUAL(slhs, srhs);

	slhs = split_map_a + val_pair;
	srhs = val_pair + split_map_a;
	BOOST_CHECK_EQUAL(slhs, srhs);

	jlhs = join_map_a + map_pair;
	jrhs = map_pair + join_map_a;
	BOOST_CHECK_EQUAL(jlhs, jrhs);

	slhs = split_map_a + map_pair;
	srhs = map_pair + split_map_a;
	BOOST_CHECK_EQUAL(slhs, srhs);

	slhs = split_map_a + join_map_b;
	srhs = join_map_b + split_map_a;
	BOOST_CHECK_EQUAL(slhs, srhs);
	/*
	quantifier_map_check_monoid_instance_plus(
		join_map_a, join_map_b, join_map_c, 
		split_map_a, split_map_b, split_map_c, 
		val_pair, map_pair);

	quantifier_map_check_monoid_instance_et(
		join_map_a, join_map_b, join_map_c, 
		split_map_a, split_map_b, split_map_c, 
		val_pair, map_pair);
	*/
	
}



#endif // __test_itl_quantifier_map_shared_h_JOFA_090119__

