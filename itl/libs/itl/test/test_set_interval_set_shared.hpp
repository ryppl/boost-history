/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_set_interval_set_h_JOFA_090119__
#define __test_itl_set_interval_set_h_JOFA_090119__



template <class T, 
          template<class T, 
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void make_3_interval_sets_and_derivatives_1
                   (IntervalSet<T>& set_a, 
					IntervalSet<T>& set_b, 
					IntervalSet<T>& set_c, 
					typename IntervalSet<T>::segment_type& segm_d,
					typename IntervalSet<T>::element_type& elem_e)
{
	typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::segment_type IntervalT;
    typedef typename IntervalSetT::segment_type segment_type;
    typedef typename IntervalSetT::element_type element_type;

    segm_d = I_D(6,9);
    elem_e = MK_v(5);

    set_a.add(I_D(3,6)).add(I_I(5,7));
    set_b.add(C_D(1,3)).add(I_D(8,9));
    set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));
}



//------------------------------------------------------------------------------
// Monoid EAN
//------------------------------------------------------------------------------
template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void interval_set_check_monoid_plus_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::interval_type IntervalT;

    IntervalSetT set_a, set_b, set_c;
    set_a.add(I_D(3,6)).add(I_I(5,7));
    set_b.add(C_D(1,3)).add(I_D(8,9));
    set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    typename IntervalSetT::segment_type inter_val1 = I_D(6,9);
    typename IntervalSetT::segment_type inter_val2 = I_I(5,5);

    CHECK_MONOID_INSTANCE_WRT(plus) (set_a, set_b, set_c, inter_val1, inter_val2);
    CHECK_MONOID_INSTANCE_WRT(pipe) (set_a, set_b, set_c, inter_val1, inter_val2);
}

template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void interval_set_check_monoid_et_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::interval_type IntervalT;

    IntervalSetT set_a, set_b, set_c;
    set_a.add(I_D(3,6)).add(I_I(5,7));
    set_b.add(C_D(1,3)).add(I_D(8,9));
    set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    typename IntervalSetT::segment_type inter_val1 = I_D(6,9);
    typename IntervalSetT::segment_type inter_val2 = I_I(5,5);

    CHECK_MONOID_INSTANCE_WRT(et)   (set_a, set_b, set_c, inter_val1, inter_val2);
    CHECK_MONOID_INSTANCE_WRT(caret)(set_a, set_b, set_c, inter_val1, inter_val2);
}

//------------------------------------------------------------------------------
// Abelian monoid EANC
//------------------------------------------------------------------------------

template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void interval_set_check_abelian_monoid_plus_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::interval_type IntervalT;

    IntervalSetT set_a, set_b, set_c;
    set_a.add(I_D(3,6)).add(I_I(5,7));
    set_b.add(C_D(1,3)).add(I_D(8,9));
    set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    typename IntervalSetT::segment_type inter_val1 = I_D(6,9);
    typename IntervalSetT::segment_type inter_val2 = I_I(5,5);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(plus) (set_a, set_b, set_c, inter_val1, inter_val2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(pipe) (set_a, set_b, set_c, inter_val1, inter_val2);
}

template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void interval_set_check_abelian_monoid_et_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::interval_type IntervalT;

    IntervalSetT set_a, set_b, set_c;
    set_a.add(I_D(3,6)).add(I_I(5,7));
    set_b.add(C_D(1,3)).add(I_D(8,9));
    set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    typename IntervalSetT::segment_type inter_val1 = I_D(6,9);
    typename IntervalSetT::segment_type inter_val2 = I_I(5,5);

    CHECK_ABELIAN_MONOID_INSTANCE_WRT(et)   (set_a, set_b, set_c, inter_val1, inter_val2);
    CHECK_ABELIAN_MONOID_INSTANCE_WRT(caret)(set_a, set_b, set_c, inter_val1, inter_val2);
}


//------------------------------------------------------------------------------
// Abelian partial invertive monoid 
//------------------------------------------------------------------------------
template <class T,
          template<class T,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void interval_set_check_partial_invertive_monoid_plus_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::interval_type IntervalT;

    IntervalSetT set_a, set_b, set_c;
    set_a.add(I_D(3,6)).add(I_I(5,7));
    set_b.add(C_D(1,3)).add(I_D(8,9));
    set_c.add(I_D(0,9)).add(I_I(3,6)).add(I_D(5,7));

    typename IntervalSetT::segment_type inter_val1 = I_D(6,9);
    typename IntervalSetT::segment_type inter_val2 = I_I(5,5);

    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(plus) (set_a, set_b, set_c, inter_val1, inter_val2);
    CHECK_PARTIAL_INVERTIVE_MONOID_INSTANCE_WRT(pipe) (set_a, set_b, set_c, inter_val1, inter_val2);
}


//------------------------------------------------------------------------------
// Containedness
//------------------------------------------------------------------------------
template <class T, 
          template<class T, 
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_INTERVAL(ITL_COMPARE)  Interval = ITL_INTERVAL_INSTANCE(ITL_INTERVAL_DEFAULT, T, Compare),
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalSet
          >
void interval_set_check_containedness_4_bicremental_types()
{
    typedef IntervalSet<T> IntervalSetT;
    typedef typename IntervalSetT::segment_type segment_type;
    typedef typename IntervalSetT::element_type element_type;

	IntervalSetT set_a, set_b, set_c;
	segment_type segm_d;
	element_type elem_e;
	make_3_interval_sets_and_derivatives_1(set_a, set_b, set_c, segm_d, elem_e);

	check_intersection_containedness(set_a, set_c);
	check_intersection_containedness(set_b, segm_d);
	check_intersection_containedness(set_c, elem_e);

	check_union_containedness(set_c, set_c);
	check_union_containedness(set_b, segm_d);
	check_union_containedness(set_a, elem_e);
}



#endif // __test_itl_set_interval_set_h_JOFA_090119__

