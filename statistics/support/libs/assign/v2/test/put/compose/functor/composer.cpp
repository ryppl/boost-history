//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>

#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/modifier/ext/repeat.hpp>
#include <boost/assign/v2/put/modifier/ext/incr_lookup.hpp>
#include <boost/assign/v2/put/compose/functor/composer.hpp>

#include <libs/assign/v2/test/put/compose/functor/composer.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_compose{
namespace xxx_functor{
namespace xxx_composer{

	void test()
    {

        using namespace boost::assign::v2;
        using namespace checking::constants;
        typedef boost::mpl::int_<0> zero_;
        typedef boost::mpl::int_<1> one_;
        typedef boost::mpl::int_<2> two_;
        {
            int x = -1;
			typedef put_compose_aux::composer<> composer_;

            BOOST_AUTO(
            	tmp,
            	( ( composer_() %  ( _repeat = 3 ) )( x ) )
            );
            int n = tmp
                .parameters()
                .static_lookup( zero_() ).get();
            BOOST_ASSIGN_V2_CHECK_EQUAL( n, 3 );

        }
		{
			typedef put_compose_aux::composer<> composer_;
            BOOST_AUTO(
            	tmp,
            	( composer_() % ( _incr_lookup = 3 ) ) 
            );
            int n = tmp
                .parameters()
                .static_lookup( zero_() ).get();
            BOOST_ASSIGN_V2_CHECK_EQUAL( n, 3 );
		}
        {
            int a1, b1, c1, d1;//, e1, f1, g1, h1;
            using namespace checking::constants;
            {
                a1 = a; b1 = b; c1 = c; d1 = d;
            }

            using namespace adaptor;
            BOOST_AUTO(
            	tmp,
                ( _put( a1, b1 )( c1 )() )
            );
            {
            	int & x = tmp.sequence_args()
            		.static_row( zero_() )
                	.static_elem( zero_() );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &a1, &x );
                BOOST_ASSIGN_V2_CHECK_EQUAL( a, x );
            }
            {
            	int & x = tmp.sequence_args()
            		.static_row( zero_() )
                	.static_elem( one_() );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &b1, &x );
                BOOST_ASSIGN_V2_CHECK_EQUAL( b, x );
            }
            {
            	int & x = tmp.sequence_args()
            		.static_row( one_() )
                	.static_elem( zero_() );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &c1, &x );
            	BOOST_ASSIGN_V2_CHECK_EQUAL( c, x );
            }
            {
            	tmp.sequence_args()
            		.static_row( two_() );
            }
        }
    }

}// xxx_composer
}// xxx_functor
}// xxx_compose
}// xxx_put
}// test_assign_v2
