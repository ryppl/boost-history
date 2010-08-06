//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <queue>
#include <vector>
#include <map>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <libs/assign/v2/test/put/put_wrapper.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_put_wrapper{

    void test()
    {
		using namespace boost::assign::v2;
        using namespace checking::constants;
        namespace chk_cont = checking::container;
        {
        	// Array
            typedef boost::array<int,8> cont_;
            cont_ cont;
            chk_cont::do_check(
                put( cont )
                    ( a )( b )( c )( d )( e )( f )( g )( h ).unwrap()
            );
        }
		{	// Queue
            typedef std::queue<int> cont_;
            cont_ cont;
            ( put( cont ) % (_repeat = 3 ) ) (-1);
            BOOST_ASSIGN_V2_CHECK_EQUAL( cont.front(), -1); cont.pop();
            BOOST_ASSIGN_V2_CHECK_EQUAL( cont.front(), -1); cont.pop();
            BOOST_ASSIGN_V2_CHECK_EQUAL( cont.front(), -1); cont.pop();
            BOOST_ASSIGN_V2_CHECK_EQUAL( cont.empty(), true);
		}
        {
        	int a1, b1, c1, d1;//, e1, f1, g1, h1;
            {
                a1 = a; b1 = b; c1 = c;
                d1 = d;
            }
            {	// Deque + tuple
            	typedef boost::tuple<int&, const int&> tuple_;
                typedef std::deque<tuple_> cont_;
                {
                	cont_ cont;
                	put( cont )( a1 , b1 )( c1 , d1 );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &a1, &boost::get<0>( cont[0] ) );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &b1, &boost::get<1>( cont[0] ) );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &c1, &boost::get<0>( cont[1] ) );
                BOOST_ASSIGN_V2_CHECK_EQUAL( &d1, &boost::get<1>( cont[1] ) );
                }
            }
            {
            	// Map
                typedef std::map<const char*, int> cont_;
                cont_ cont;
                put( cont )( "x", 2 )( "y", 1 );
            }
            {	// Map
                typedef std::map<const char*, int> cont_;
                cont_ cont;
                ( put( cont ) % _incr_lookup).csv( "x", "y", "x" );
                BOOST_ASSIGN_V2_CHECK_EQUAL( 2, cont["x"] );
                BOOST_ASSIGN_V2_CHECK_EQUAL( 1, cont["y"] );
            }
        }
    }


}// xxx_put_wrapper
}// xxx_put
}// test_assign_v2


