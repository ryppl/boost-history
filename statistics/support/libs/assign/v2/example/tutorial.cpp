////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <map>
#include <vector>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

#include <boost/range.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/v2.hpp>

#include <libs/assign/v2/example/tutorial.h>

namespace example_assign_v2{
namespace xxx_tutorial{

    void run()
    {
    	using namespace boost::assign::v2;
        {
        	// --- PUT --- //
        	{
            	typedef boost::tuple<int&, const int&> tuple_;
            	typedef std::deque<tuple_> cont_;
            	cont_ cont;
            	int a1, a2, b1, c1;
            	( put( cont ) % ( _fun = boost::lambda::bind<tuple_>(
            		constructor<tuple_>(),
            		boost::lambda::_1,
            	    a2
           	 	) ) )( a1 )( b1 )( c1 );
        	}
        	{
            	typedef std::map<const char*, int> cont_;
                typedef boost::range_value<cont_>::type value_;
                cont_ cont;
            	( put( cont ) % ( _incr_lookup = 2 ) )( "x" )( "y" )( "x" );
                BOOST_ASSERT( cont["x"] == 4 );
                BOOST_ASSERT( cont["y"] == 2 );
        	}
        }
        {
        	// --- ANON --- //
        	{
            	typedef std::vector<int> cont_;
                cont_ cont = csv_anon( 1 )( 2 )( 3 ).convert<cont_>();
                BOOST_ASSERT( cont[0] == 1 );
                BOOST_ASSERT( cont[2] == 3 );
        	}
        }
        {
        	// --- REF-ANON-- //
			using namespace ref;
            {	// -- assign_copy-- //
            	std::vector<int> v( 3 );
                v[ 0 ] = 1;
                v[ 1 ] = 2;
                v[ 2 ] = 3;
            	int a, b, c;
				boost::copy( v ,
                	boost::begin( assign_copy::anon( a )( b )( c ) ) );
				BOOST_ASSERT( a == v[ 0 ] );
				BOOST_ASSERT( b == v[ 1 ] );
				BOOST_ASSERT( c == v[ 2 ] );
            }
			{
				int a = 1, b = 2, c = 3;
				BOOST_AUTO( tmp, assign_rebind::csv_anon( a , b ) );
				tmp.assign( c );
				BOOST_ASSERT( &tmp[ 0 ] == &c );
				BOOST_ASSERT( &tmp[ 1 ] == &c );
			}
        }
        {	// --- CHAIN --- //
        	using namespace ref;
        	typedef boost::array<int,3> cont_;
            cont_ cont; cont[ 0 ] = 1; cont[ 1 ] = 2; cont[ 2 ] = 3;
            int a, b, c = 3;
			BOOST_AUTO( tmp, assign_copy::anon( a )( b ) );
			boost::copy(
                cont , boost::begin( tmp && assign_copy::anon( c ) ) );
			BOOST_ASSERT( a == cont[ 0 ] );
			BOOST_ASSERT( b == cont[ 1 ] );
			BOOST_ASSERT( c == cont[ 2 ] );
        }
    }
}// xxx_tutorial
}// example_assign_v2

