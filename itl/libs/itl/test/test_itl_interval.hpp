/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_ITL_INTERVAL_HPP_JOFA_081006
#define BOOST_ITL_TEST_ITL_INTERVAL_HPP_JOFA_081006

template <class T, class IntervalT> 
void interval_ctor_4_ordered_types()
{
    T lower_bound = given<mpl::or_<is_static_right_open<IntervalT>, is_static_open<IntervalT> >, T>
                        ::then(identity_element<T>::value(), unit_element<T>::value());

    typedef typename domain_type_of<IntervalT>::type Dom1T;
    typedef typename domain_type_of<interval_traits<IntervalT> >::type Dom2T;
    BOOST_CHECK_EQUAL(is_same<Dom1T, Dom2T>::value, true);


    // An empty interval is defined as the closed interval [1,0]
    BOOST_CHECK_EQUAL(itl::is_empty(IntervalT()), true);
    BOOST_CHECK_EQUAL(itl::cardinality(IntervalT()), itl::identity_element<typename itl::size_type_of<T>::type>::value());
    BOOST_CHECK_EQUAL(itl::size(IntervalT()), itl::identity_element<typename itl::size_type_of<T>::type>::value());
    BOOST_CHECK_EQUAL(IntervalT().lower(), lower_bound);
    BOOST_CHECK_EQUAL(IntervalT().upper(), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(itl::lower(IntervalT()), lower_bound);
    BOOST_CHECK_EQUAL(itl::upper(IntervalT()), itl::identity_element<T>::value());

    BOOST_CHECK_EQUAL(IntervalT(), IntervalT());
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT(lower_bound, itl::identity_element<T>::value()));
}

/*JODO
*/

#endif // __test_itl_interval_hpp_JOFA_081006__
