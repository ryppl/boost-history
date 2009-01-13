/*----------------------------------------------------------------------------+
Copyright (c) 2008-2008: Joachim Faulhaber
+-----------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+----------------------------------------------------------------------------*/
#define BOOST_TEST_MODULE itl::interval unit test
#include <string>
#include <boost/mpl/list.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_case_template.hpp>

// interval instance types
#include "../test_type_lists.hpp"
#include "../test_value_maker.hpp"

#include <boost/itl/interval.hpp>

using namespace std;
using namespace boost;
using namespace unit_test;
using namespace boost::itl;


// Most general, largest set of types
BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_ctor_4_ordered_types, T, ordered_types)
{
    // An empty interval is defined as the closed interval [1,0]
    BOOST_CHECK_EQUAL(interval<T>().empty(), true);
    BOOST_CHECK_EQUAL(interval<T>().cardinality(), itl::neutron<typename itl::size<T>::type>::value());
    BOOST_CHECK_EQUAL(interval<T>().size(), itl::neutron<typename itl::size<T>::type>::value());
    //JODO STATIC_ASSERTS length fails with std::string
    //BOOST_CHECK_EQUAL(interval<T>().length(), itl::neutron<itl::typename difference<T>::type>::value());
    BOOST_CHECK_EQUAL(interval<T>().lower(), itl::unon<T>::value());
    BOOST_CHECK_EQUAL(interval<T>().upper(), itl::neutron<T>::value());

    BOOST_CHECK_EQUAL(interval<T>(), interval<T>());
    BOOST_CHECK_EQUAL(interval<T>(), interval<T>(itl::unon<T>::value(), itl::neutron<T>::value()));
    BOOST_CHECK_EQUAL(interval<T>(), interval<T>(itl::unon<T>::value(), itl::neutron<T>::value(), interval<T>::CLOSED));

}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_ctor_4_bicremental_types, T, bicremental_types)
{
    BOOST_CHECK_EQUAL( T(), pred(succ(T())));
    BOOST_CHECK_EQUAL( itl::neutron<T>::value(), pred(succ(itl::neutron<T>::value()))       );
    BOOST_CHECK_EQUAL( itl::unon<T>::value(),    succ(itl::neutron<T>::value())             );
    BOOST_CHECK_EQUAL( interval<T>().length(),   itl::neutron<typename difference<T>::type>::value() );

    T v4 = make<T>(4);
    itl::interval<T> I4_4I(v4);
    BOOST_CHECK_EQUAL( I4_4I.is_closed(),         true  );
    BOOST_CHECK_EQUAL( I4_4I.is_leftopen(),       false );
    BOOST_CHECK_EQUAL( I4_4I.is_rightopen(),      false );
    BOOST_CHECK_EQUAL( I4_4I.is_open(),           false );
    BOOST_CHECK_EQUAL( I4_4I.leftbound_closed(),  true  );
    BOOST_CHECK_EQUAL( I4_4I.rightbound_closed(), true  );
    BOOST_CHECK_EQUAL( I4_4I.leftbound_open(),    false );
    BOOST_CHECK_EQUAL( I4_4I.rightbound_open(),   false );

    BOOST_CHECK_EQUAL( I4_4I.lower(),             v4    );
    BOOST_CHECK_EQUAL( I4_4I.upper(),             v4    );

    BOOST_CHECK_EQUAL( I4_4I.contains(v4),        true  );
    BOOST_CHECK_EQUAL( I4_4I.contains(I4_4I),     true  );
    BOOST_CHECK_EQUAL( I4_4I.contained_in(I4_4I), true  );
    BOOST_CHECK_EQUAL( I4_4I,                     I4_4I );

    BOOST_CHECK_EQUAL( I4_4I.cardinality(),       unon<typename interval<T>::size_type>::value()          );
    BOOST_CHECK_EQUAL( I4_4I.size(),              unon<typename interval<T>::size_type>::value()          );
    //BOOST_CHECK_EQUAL( I4_4I.length(),          neutron<typename interval<T>::difference_type>::value() );

    itl::interval<T> j_4_4(I4_4I);
    BOOST_CHECK_EQUAL( I4_4I, j_4_4 );
    interval<T> k_4_4;
    k_4_4 = j_4_4;
    BOOST_CHECK_EQUAL( I4_4I, k_4_4 );

    T v2 = make<T>(2);
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4),    interval<T>(v2, v4) );
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4),    interval<T>(v2, v4, interval<T>::CLOSED) );
    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4), interval<T>(v2, v4, interval<T>::RIGHT_OPEN) );
    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4),  interval<T>(v2, v4, interval<T>::LEFT_OPEN) );
    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4),      interval<T>(v2, v4, interval<T>::OPEN) );

    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).lower(),               v2 );
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).upper(),               v4 );
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).boundtypes(),          interval<T>::CLOSED );
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).is_closed(),           true );
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).leftbound_closed(),    true );
    BOOST_CHECK_EQUAL( interval<T>::closed(v2, v4).rightbound_closed(),   true );

    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).lower(),            v2 );
    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).upper(),            v4 );
    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).boundtypes(),       interval<T>::RIGHT_OPEN );
    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).is_rightopen(),     true );
    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).leftbound_closed(), true );
    BOOST_CHECK_EQUAL( interval<T>::rightopen(v2, v4).rightbound_open(),  true );

    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).lower(),             v2 );
    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).upper(),             v4 );
    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).boundtypes(),        interval<T>::LEFT_OPEN );
    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).is_leftopen(),       true );
    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).leftbound_open(),    true );
    BOOST_CHECK_EQUAL( interval<T>::leftopen(v2, v4).rightbound_closed(), true );

    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).lower(),                 v2 );
    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).upper(),                 v4 );
    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).boundtypes(),            interval<T>::OPEN );
    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).is_open(),               true );
    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).leftbound_open(),        true );
    BOOST_CHECK_EQUAL( interval<T>::open(v2, v4).rightbound_open(),       true );    
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_ctor_4_integral_types, T, integral_types)
{
    BOOST_CHECK_EQUAL(interval<T>().first(),  itl::unon<T>::value());
    BOOST_CHECK_EQUAL(interval<T>().last(),   itl::neutron<T>::value());
    BOOST_CHECK_EQUAL(interval<T>().length(), itl::neutron<typename interval<T>::difference_type>::value());

    BOOST_CHECK_EQUAL(interval<T>(0,0).length(), itl::unon<typename interval<T>::difference_type>::value());
}

BOOST_AUTO_TEST_CASE(test_itl_interval_ctor_specific)
{
    BOOST_CHECK_EQUAL(interval<double>().continuous_length(), 0.0);
    BOOST_CHECK_EQUAL(interval<double>(5.0,5.0).cardinality(), 1);
    BOOST_CHECK_EQUAL(interval<std::string>("test","test").cardinality(), 1);
    BOOST_CHECK_EQUAL(interval<std::string>("best","test").cardinality(), interval<double>(0.0,0.1).cardinality());
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_equal_4_integral_types, T, integral_types)
{
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v7 = make<T>(7);
    T v8 = make<T>(8);
    BOOST_CHECK_EQUAL(interval<T>(), interval<T>(v7,v3));

    //I: (I)nside  = closed bound
    //C: left open bound
    //D: right open bound
    interval<T>  I3_7I  = interval<T>::closed(v3,v7);
    interval<T>  I3__8D = interval<T>::rightopen(v3,v8);
    interval<T> C2__7I  = interval<T>::leftopen(v2,v7);
    interval<T> C2___8D = interval<T>::open(v2,v8);

    BOOST_CHECK_EQUAL(  I3_7I ,  I3_7I  );    
    BOOST_CHECK_EQUAL(  I3_7I ,  I3__8D );    
    BOOST_CHECK_EQUAL(  I3_7I , C2__7I  );    
    BOOST_CHECK_EQUAL(  I3_7I , C2___8D );    

    BOOST_CHECK_EQUAL(  I3__8D,  I3__8D );    
    BOOST_CHECK_EQUAL(  I3__8D, C2__7I  );    
    BOOST_CHECK_EQUAL(  I3__8D, C2___8D );    

    BOOST_CHECK_EQUAL( C2__7I , C2__7I  );    
    BOOST_CHECK_EQUAL( C2__7I , C2___8D );    

    BOOST_CHECK_EQUAL( C2___8D, C2___8D );    
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_equal_4_bicremental_continuous_types, T, bicremental_continuous_types)
{
    T v3 = make<T>(3);
    T v7 = make<T>(7);
    BOOST_CHECK_EQUAL(interval<T>(), interval<T>(v7,v3));

    //I: (I)nside  = closed bound
    //O: (O)utside = open bound
    interval<T> I3_7I = interval<T>::closed(v3,v7);
    interval<T> I3_7D = interval<T>::rightopen(v3,v7);
    interval<T> C3_7I = interval<T>::leftopen(v3,v7);
    interval<T> C3_7D = interval<T>::open(v3,v7);

    BOOST_CHECK_EQUAL( I3_7I ,  I3_7I  );    
    BOOST_CHECK_EQUAL( I3_7I == I3_7D, false  );    
    BOOST_CHECK_EQUAL( I3_7I == C3_7D, false  );    
    BOOST_CHECK_EQUAL( I3_7I == C3_7D, false );    

    BOOST_CHECK_EQUAL( I3_7D ,  I3_7D  );    
    BOOST_CHECK_EQUAL( I3_7D == C3_7I, false  );    
    BOOST_CHECK_EQUAL( I3_7D == C3_7D, false );    

    BOOST_CHECK_EQUAL( C3_7I ,  C3_7I  );    
    BOOST_CHECK_EQUAL( C3_7I == C3_7D, false );    

    BOOST_CHECK_EQUAL( C3_7D,   C3_7D  );    
} 

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_touches_4_bicremental_types, T, bicremental_types)
{
    T v3 = make<T>(3);
    T v7 = make<T>(7);
    T v9 = make<T>(9);

    interval<T> I3_7D = interval<T>::rightopen(v3,v7);
    interval<T> I7_9I = interval<T>::closed(v7,v9);
    BOOST_CHECK_EQUAL( I3_7D.touches(I7_9I), true );    

    interval<T> I3_7I = interval<T>::closed(v3,v7);
    interval<T> C7_9I = interval<T>::leftopen(v7,v9);
    BOOST_CHECK_EQUAL( I3_7I.touches(C7_9I), true );

    BOOST_CHECK_EQUAL( I3_7D.touches(C7_9I), false );    
    BOOST_CHECK_EQUAL( I3_7I.touches(I7_9I), false );    
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_touches_4_integral_types, T, integral_types)
{
    T v3 = make<T>(3);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v9 = make<T>(9);

    interval<T> I3_6I = interval<T>::closed(v3,v6);
    interval<T> I7_9I = interval<T>::closed(v7,v9);
    BOOST_CHECK_EQUAL( I3_6I.touches(I7_9I), true );    

    interval<T> I3_7D = interval<T>::rightopen(v3,v7);
    interval<T> C6_9I = interval<T>::leftopen(v6,v9);
    BOOST_CHECK_EQUAL( I3_7D.touches(C6_9I), true );
}


BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_inplace_intersect_4_bicremental_types, T, bicremental_types)
{
    T v0 = make<T>(0);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v9 = make<T>(9);

    interval<T> section;
    interval<T> I3_7D = interval<T>::rightopen(v3,v7);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    section = I3_7D; section &= I0_3D;
    BOOST_CHECK_EQUAL( I0_3D.is_disjoint(I3_7D), true );
    BOOST_CHECK_EQUAL( section.empty(), true );
    BOOST_CHECK_EQUAL( section, interval<T>() );

    interval<T> I0_5D = interval<T>::rightopen(v0,v5);
    section = I3_7D; section &= I0_5D;
    BOOST_CHECK_EQUAL( section, interval<T>::rightopen(v3, v5) );

    interval<T> I0_9D = interval<T>::rightopen(v0,v9);
    section = I3_7D; section &= I0_9D;
    BOOST_CHECK_EQUAL( section, I3_7D );

    interval<T> I4_5I = interval<T>::closed(v4,v5);
    section = I3_7D; section &= I4_5I;
    BOOST_CHECK_EQUAL( section, I4_5I );

    interval<T> C4_6D = interval<T>::open(v4,v6);
    section = I3_7D; section &= C4_6D;
    BOOST_CHECK_EQUAL( section, C4_6D );

    interval<T> C4_9I = interval<T>::leftopen(v4,v9);
    section = I3_7D; section &= C4_9I;
    BOOST_CHECK_EQUAL( section, interval<T>::open(v4,v7) );

    interval<T> I7_9I = interval<T>::closed(v7,v9);
    section = I3_7D; section &= I7_9I;
    BOOST_CHECK_EQUAL( I3_7D.exclusive_less(I7_9I), true );
    BOOST_CHECK_EQUAL( I3_7D.is_disjoint(I7_9I), true );
    BOOST_CHECK_EQUAL( section.empty(), true );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_infix_intersect_4_bicremental_types, T, bicremental_types)
{
    T v0 = make<T>(0);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v9 = make<T>(9);

    interval<T> section;
    interval<T> I3_7D = interval<T>::rightopen(v3,v7);

    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    section = I3_7D & I0_3D;
    BOOST_CHECK_EQUAL( I0_3D.is_disjoint(I3_7D), true );
    BOOST_CHECK_EQUAL( section.empty(), true );
    BOOST_CHECK_EQUAL( section, interval<T>() );

    interval<T> I0_5D = interval<T>::rightopen(v0,v5);
    section = I3_7D & I0_5D;
    BOOST_CHECK_EQUAL( section, interval<T>::rightopen(v3, v5) );

    interval<T> I0_9D = interval<T>::rightopen(v0,v9);
    section = I3_7D & I0_9D;
    BOOST_CHECK_EQUAL( section, I3_7D );

    interval<T> I4_5I = interval<T>::closed(v4,v5);
    section = I3_7D & I4_5I;
    BOOST_CHECK_EQUAL( section, I4_5I );

    interval<T> C4_6D = interval<T>::open(v4,v6);
    section = I3_7D & C4_6D;
    BOOST_CHECK_EQUAL( section, C4_6D );

    interval<T> C4_9I = interval<T>::leftopen(v4,v9);
    section = I3_7D & C4_9I;
    BOOST_CHECK_EQUAL( section, interval<T>::open(v4,v7) );

    interval<T> I7_9I = interval<T>::closed(v7,v9);
    section = I3_7D & I7_9I;
    BOOST_CHECK_EQUAL( I3_7D.exclusive_less(I7_9I), true );
    BOOST_CHECK_EQUAL( I3_7D.is_disjoint(I7_9I), true );
    BOOST_CHECK_EQUAL( section.empty(), true );
}

BOOST_AUTO_TEST_CASE_TEMPLATE(test_itl_interval_subtract_4_bicremental_types, T, bicremental_types)
{
    T v0 = make<T>(0);
    T v2 = make<T>(2);
    T v3 = make<T>(3);
    T v4 = make<T>(4);
    T v5 = make<T>(5);
    T v6 = make<T>(6);
    T v7 = make<T>(7);
    T v9 = make<T>(9);

    interval<T> diff_1, diff_2;
    interval<T> I0_3D = interval<T>::rightopen(v0,v3);
    interval<T> I2_6D = interval<T>::rightopen(v2,v6);
    interval<T> I4_7D = interval<T>::rightopen(v4,v7);
    interval<T> I6_7D = interval<T>::rightopen(v6,v7);
    interval<T> I2_4D = interval<T>::rightopen(v2,v4);

	I2_6D.right_subtract(diff_1,I4_7D);
    BOOST_CHECK_EQUAL( diff_1, I2_4D );

	diff_2 = I2_6D;
	diff_2.right_subtract(I4_7D);
    BOOST_CHECK_EQUAL( diff_2, I2_4D );

	diff_1.clear();
	I0_3D.right_subtract(diff_1, I4_7D);
    BOOST_CHECK_EQUAL( diff_1, I0_3D );
	
	// ---------------------------------
	I4_7D.left_subtract(diff_1, I2_6D);
    BOOST_CHECK_EQUAL( diff_1, I6_7D );

	diff_2 = I4_7D;
	diff_2.left_subtract(I2_6D);
    BOOST_CHECK_EQUAL( diff_2, I6_7D );

	diff_1.clear();
	I4_7D.left_subtract(diff_1, I0_3D);
    BOOST_CHECK_EQUAL( diff_1, I4_7D );

	diff_2 = I4_7D;
	diff_2.left_subtract(I0_3D);
    BOOST_CHECK_EQUAL( diff_2, I4_7D );

}
