/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TEST_ICL_INTERVAL_HPP_JOFA_100930
#define BOOST_ITL_TEST_ICL_INTERVAL_HPP_JOFA_100930

// NOTE: ordered_types is the largest class of types that is admissable as 
//                     domain parameters for intervals and interval containers.
//   bicremental_types is a smaller class of types used for testing instead of
//                     ordered types, because they can be generated in a simple
//                     way using the functions of test_value_maker.hpp.

template <class IntervalT> 
void interval_ctor_4_ordered_types()
{
    typedef typename domain_type_of<interval_traits<IntervalT> >::type T;
    typedef typename itl::size_type_of<T>::type SizeT;
    T t_0     = itl::neutron<T>::value();
    T t_1     = itl::unon<T>::value();
    SizeT s_0 = itl::neutron<SizeT>::value();
    SizeT s_1 = itl::unon<SizeT>::value();

    // Default ctor and emptieness
    BOOST_CHECK_EQUAL( itl::is_empty(IntervalT()), true );
    BOOST_CHECK_EQUAL( itl::cardinality(IntervalT()), s_0 );
    BOOST_CHECK_EQUAL(        itl::size(IntervalT()), s_0 );

    BOOST_CHECK_EQUAL( IntervalT(), IntervalT() );
    BOOST_CHECK_EQUAL( IntervalT(), IntervalT(IntervalT().lower(), IntervalT().upper()) );
    BOOST_CHECK_EQUAL( IntervalT(), IntervalT(itl::lower(IntervalT()), itl::upper(IntervalT())) );

    BOOST_CHECK_EQUAL(itl::cardinality(IntervalT(t_0, t_1)) >= s_1,  true);
    BOOST_CHECK_EQUAL((    itl::contains(IntervalT(t_0, t_1), t_0)
                       ||  itl::contains(IntervalT(t_0, t_1), t_1)), true);

    BOOST_CHECK_EQUAL(IntervalT(t_0, t_1).lower(),  t_0);
    BOOST_CHECK_EQUAL(IntervalT(t_0, t_1).upper(),  t_1);
    BOOST_CHECK_EQUAL(lower(itl::construct<IntervalT>(t_0, t_1)),  t_0);
    BOOST_CHECK_EQUAL(upper(itl::construct<IntervalT>(t_0, t_1)),  t_1);
}


template <class IntervalT> 
void singelizable_interval_4_ordered_types()
{
    // Singleton ctor and singleness 
    // LAW:  !is_asymmetric_continuous(IntervalT) => size(singleton(x))==1
    // LAW: This law applies to all discrete and to dynamic continuous intervals
    // LAW: No singletons can be constructed for static continuous rightopen and leftopen intervals
    typedef typename domain_type_of<interval_traits<IntervalT> >::type T;
    typedef typename itl::size_type_of<T>::type SizeT;
    T t_0     = itl::neutron<T>::value();
    T t_1     = itl::unon<T>::value();
    SizeT s_1 = itl::unon<SizeT>::value();

#ifdef BOOST_MSVC 
    BOOST_CHECK_EQUAL( is_singelizable<IntervalT>::value, true ); 
#endif                        

    BOOST_CHECK_EQUAL( itl::cardinality(itl::singleton<IntervalT>(t_0)), s_1 ); 
    BOOST_CHECK_EQUAL(        itl::size(itl::singleton<IntervalT>(t_0)), s_1 ); 
    BOOST_CHECK_EQUAL( itl::cardinality(itl::singleton<IntervalT>(t_1)), s_1 ); 
    BOOST_CHECK_EQUAL(        itl::size(itl::singleton<IntervalT>(t_1)), s_1 ); 

    BOOST_CHECK_EQUAL( itl::contains(itl::singleton<IntervalT>(t_0), t_0), true ); 
    BOOST_CHECK_EQUAL( itl::contains(itl::singleton<IntervalT>(t_1), t_1), true ); 
}

template <class IntervalT> 
void singelizable_interval_4_bicremental_types()
{
    typedef typename domain_type_of<interval_traits<IntervalT> >::type T;
    typedef typename itl::size_type_of<T>::type SizeT;
    //T t_0     = itl::neutron<T>::value();
    SizeT s_1 = itl::unon<SizeT>::value();

#ifdef BOOST_MSVC 
    BOOST_CHECK_EQUAL( is_singelizable<IntervalT>::value, true ); 
#endif                        

    BOOST_CHECK_EQUAL( itl::cardinality(IntervalT(MK_v(3))), s_1 ); 
    BOOST_CHECK_EQUAL(        itl::size(IntervalT(MK_v(4))), s_1 ); 
    BOOST_CHECK_EQUAL( itl::singleton<IntervalT>(MK_v(2)), itl::singleton<IntervalT>(MK_v(2)) );
    BOOST_CHECK_EQUAL( itl::contains(IntervalT(MK_v(1)), MK_v(1)), true ); 
}



#endif // BOOST_ITL_TEST_ICL_INTERVAL_HPP_JOFA_100930
