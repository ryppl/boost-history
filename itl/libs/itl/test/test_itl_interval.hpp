/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ICL_TEST_ICL_INTERVAL_HPP_JOFA_081006
#define BOOST_ICL_TEST_ICL_INTERVAL_HPP_JOFA_081006

template <class T, class IntervalT> 
void interval_ctor_4_ordered_types()
{
    T lower_bound = given<mpl::or_<is_static_right_open<IntervalT>, is_static_open<IntervalT> >, T>
                        ::then(identity_element<T>::value(), unit_element<T>::value());

    typedef typename domain_type_of<IntervalT>::type Dom1T;
    typedef typename domain_type_of<interval_traits<IntervalT> >::type Dom2T;
    BOOST_CHECK_EQUAL(is_same<Dom1T, Dom2T>::value, true);


    // An empty interval is defined as the closed interval [1,0]
    BOOST_CHECK_EQUAL(icl::is_empty(IntervalT()), true);
    BOOST_CHECK_EQUAL(icl::cardinality(IntervalT()), icl::identity_element<typename icl::size_type_of<T>::type>::value());
    BOOST_CHECK_EQUAL(icl::size(IntervalT()), icl::identity_element<typename icl::size_type_of<T>::type>::value());
    BOOST_CHECK_EQUAL(IntervalT().lower(), lower_bound);
    BOOST_CHECK_EQUAL(IntervalT().upper(), icl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(icl::lower(IntervalT()), lower_bound);
    BOOST_CHECK_EQUAL(icl::upper(IntervalT()), icl::identity_element<T>::value());

    BOOST_CHECK_EQUAL(IntervalT(), IntervalT());
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT(lower_bound, icl::identity_element<T>::value()));
}

#endif // __test_itl_interval_hpp_JOFA_081006__
