/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_ICL_DYNAMIC_INTERVAL_HPP_JOFA_100930
#define BOOST_ITL_TEST_ICL_DYNAMIC_INTERVAL_HPP_JOFA_100930

template <class IntervalT> 
void dynamic_interval_ctor_4_ordered_types()
{
    typedef typename domain_type_of<interval_traits<IntervalT> >::type T;

    BOOST_CHECK_EQUAL(IntervalT().lower(), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(IntervalT().upper(), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(itl::lower(IntervalT()), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(itl::upper(IntervalT()), itl::identity_element<T>::value());

    IntervalT itv = IntervalT(itl::identity_element<T>::value(), itl::identity_element<T>::value());
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT(itl::identity_element<T>::value(), itl::identity_element<T>::value()));
    BOOST_CHECK_EQUAL(IntervalT(), IntervalT(itl::identity_element<T>::value(), itl::identity_element<T>::value(), interval_bounds::right_open()));
}



#endif // BOOST_ITL_TEST_ICL_DYNAMIC_INTERVAL_HPP_JOFA_100930
