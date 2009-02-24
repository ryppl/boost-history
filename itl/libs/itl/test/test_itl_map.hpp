/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef __test_itl_itl_map_h_JOFA_090119__
#define __test_itl_itl_map_h_JOFA_090119__


//------------------------------------------------------------------------------
// Monoid EAN
//------------------------------------------------------------------------------
template <class T, class U, class Trt,
          template<class T, class U,
                   class Traits = Trt,
                   ITL_COMPARE Compare = ITL_COMPARE_INSTANCE(std::less, U),
                   ITL_COMBINE Combine = ITL_COMBINE_INSTANCE(itl::inplace_plus, U),
                   ITL_SECTION Section = ITL_SECTION_INSTANCE(itl::inplace_et, U),
                   template<class,ITL_COMPARE>class Interval = interval,
                   ITL_ALLOC   Alloc   = std::allocator
                  >class IntervalMap
          >
void itl_map_find_4_bicremental_types()
{
    typedef IntervalMap<T,U,Trt> IntervalMapT;
    typedef itl::map<T,U,Trt> MapT;

    IntervalMapT itv_map_a;
    itv_map_a.add(IDv(2,4,1)).add(IIv(6,7,3));

    MapT map_a;
    Interval::atomize(map_a, itv_map_a);

    MapT::const_iterator found = map_a.find(MK_v(6));

    BOOST_CHECK_EQUAL( found->CONT_VALUE, MK_u(3) );
    BOOST_CHECK_EQUAL( map_a(MK_v(6)), MK_u(3) );

    found = map_a.find(MK_v(5));

    BOOST_CHECK_EQUAL( found == map_a.end(), true );
    BOOST_CHECK_EQUAL( map_a(MK_v(5)), MK_u(0) );
}

#endif // __test_itl_itl_map_h_JOFA_090119__

