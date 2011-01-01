
// Copyright Aleksey Gurtovoy 2001-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id: arithmetic.cpp 49240 2008-10-10 09:21:07Z agurtovoy $
// $Date: 2008-10-10 11:21:07 +0200 (ven., 10 oct. 2008) $
// $Revision: 49240 $

#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/aux_/test.hpp>

MPL_TEST_CASE()
{
{
    typedef int_<0> _0;
    typedef int_<1> _1;
    typedef int_<3> _3;
    typedef int_<4> _4;
    typedef int_<6> _6;
    typedef int_<7> _7;
    typedef int_<10> _10;
    typedef int_<18> _18;
    typedef int_<30> _30;
    typedef int_<42> _42;
    typedef int_<-1> m1;
    typedef int_<-3> m3;
    typedef int_<-6> m6;
    typedef int_<-7> m7;
    typedef int_<-9> m9;
    typedef int_<-10> m10;

    MPL_ASSERT_RELATION( (plus<_0,_10>::value), ==, 10 );
    MPL_ASSERT_RELATION( (plus<_10,_0>::value), ==, 10 );

    MPL_ASSERT_RELATION( (minus<_0,_10>::value), ==, -10 );
    MPL_ASSERT_RELATION( (minus<_10,_0>::value), ==, 10 );

    MPL_ASSERT_RELATION( (times<_1,_10>::value), ==, 10 );
    MPL_ASSERT_RELATION( (times<_10,_1>::value), ==, 10 );
    MPL_ASSERT_RELATION( (multiplies<_1,_10>::value), ==, 10 );
    MPL_ASSERT_RELATION( (multiplies<_10,_1>::value), ==, 10 );

    MPL_ASSERT_RELATION( (divides<_10,_1>::value), ==, 10 );
    MPL_ASSERT_RELATION( (divides<_10,_1>::value), ==, 10 );

    MPL_ASSERT_RELATION( (modulus<_10,_1>::value), ==, 0 );
    MPL_ASSERT_RELATION( (modulus<_10,_3>::value), ==, 1 );

#if !defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
    MPL_ASSERT_RELATION( (minus<_10,_1,_10>::value), ==, -1 );
    MPL_ASSERT_RELATION( (plus<_10,_1,_10>::value), ==, 21 );
    MPL_ASSERT_RELATION( (divides<_10,_1,_10>::value), ==, 1 );
    MPL_ASSERT_RELATION( (divides<_10,_1,_10>::value), ==, 1 );
#endif
    
    MPL_ASSERT_RELATION( negate<_10>::value, ==, -10 );
    
    MPL_ASSERT_RELATION( abs<_10>::value, ==, 10 );
    MPL_ASSERT_RELATION( abs<_0>::value, ==, 0 );
    MPL_ASSERT_RELATION( abs<m10>::value, ==, 10 );
    
    MPL_ASSERT_RELATION( sign<_10>::value, ==, 1 );
    MPL_ASSERT_RELATION( sign<_0>::value, ==, 0 );
    MPL_ASSERT_RELATION( sign<m10>::value, ==, -1 );
    
    MPL_ASSERT_RELATION( (gcd<_1,m1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (gcd<m1,_1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (gcd<_1,_1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (gcd<m1,m1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (gcd<_0,_0>::value), ==, 0 );
    MPL_ASSERT_RELATION( (gcd<_7,_0>::value), ==, 7 );
    MPL_ASSERT_RELATION( (gcd<_0,_6>::value), ==, 6 );
    MPL_ASSERT_RELATION( (gcd<m7,_0>::value), ==, 7 );
    MPL_ASSERT_RELATION( (gcd<_0,m6>::value), ==, 6 );
    MPL_ASSERT_RELATION( (gcd<_42,_30>::value), ==, 6 );
    MPL_ASSERT_RELATION( (gcd<_6,m9>::value), ==, 3 );
    MPL_ASSERT_RELATION( (gcd<m6,m6>::value), ==, 6 );
    MPL_ASSERT_RELATION( (gcd<_3,_7>::value), ==, 1 );

    MPL_ASSERT_RELATION( (lcm<_1,m1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (lcm<m1,_1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (lcm<_1,_1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (lcm<m1,m1>::value), ==, 1 );
    MPL_ASSERT_RELATION( (lcm<_0,_0>::value), ==, 0 );
    MPL_ASSERT_RELATION( (lcm<_7,_0>::value), ==, 0 );
    MPL_ASSERT_RELATION( (lcm<_0,_6>::value), ==, 0 );
    MPL_ASSERT_RELATION( (lcm<m7,_0>::value), ==, 0 );
    MPL_ASSERT_RELATION( (lcm<_0,m6>::value), ==, 0 );
    MPL_ASSERT_RELATION( (lcm<_18,_30>::value), ==, 90 );
    MPL_ASSERT_RELATION( (lcm<_6,m9>::value), ==, 18 );
    MPL_ASSERT_RELATION( (lcm<m6,m6>::value), ==, 6 );
    MPL_ASSERT_RELATION( (lcm<_3,_7>::value), ==, 21 );
}
{
    typedef integral_c<unsigned int, 0u> ui0;
    typedef long_<10> sl10;
    typedef integral_c<unsigned int, 10u> ui10;

    MPL_ASSERT_RELATION( (plus<ui0,sl10>::value), ==, 10 );
    
    MPL_ASSERT_RELATION( abs<ui10>::value, ==, 10 );
    MPL_ASSERT_RELATION( abs<ui0>::value, ==, 0 );

    MPL_ASSERT_RELATION( sign<ui10>::value, ==, 1 );
    MPL_ASSERT_RELATION( sign<ui0>::value, ==, 0 );

    MPL_ASSERT_RELATION( (gcd<ui10,ui10>::value), ==, 10 );
    MPL_ASSERT_RELATION( (gcd<ui10,sl10>::value), ==, 10 );

    MPL_ASSERT_RELATION( (lcm<ui10,sl10>::value), ==, 10 );
    
}
}
