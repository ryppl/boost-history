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
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/keyword/keyword.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/relational_op.hpp>

#include <boost/assign/v2/anon/anon.hpp>

#include <libs/assign/v2/test/anon/anon.h>

namespace test_assign_v2{
namespace xxx_anon{
namespace xxx_anon{

	void test()
    {
    	using namespace boost::assign::v2;
        {
        	// CSV
            using namespace checking::constants;
            using namespace checking::container;
            do_check( anon<int>( _nil ).csv( a,  b , c , d , e , f , g , h ) );
        }
        {
        	// Operator%
            BOOST_AUTO(tmp, ( anon<int>( _nil ) % (_repeat = 3) ) );
            tmp( 1 );
            BOOST_ASSIGN_V2_CHECK_EQUAL( boost::size( tmp ), 3 );
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.front(), 1 );
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.back(), 1 );
        }
        {	
        	// Conversion
        	typedef result_of::anon<int>::type anon_;
            anon_ from;
            {	
            	using namespace checking::constants;
            	from = anon( a )( b )( c )( d )( e )( f )( g )( h );
            }
        	using namespace checking::container;
        	{
        		typedef boost::array<int,8> to_;
            	do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
        	{
        		typedef std::deque<int> to_;
            	do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
        	{
        		typedef std::list<int> to_;
            	do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
        	{
        		typedef std::queue<int> to_;
            	do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
        	{
        		typedef std::set<int> to_;
            	do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
        	{
        		typedef std::stack<int> to_;
            	do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
	        {
    	    	typedef std::vector<int> to_;
        	    do_check( from.convert<to_>() );
                to_ to; to = from;
                do_check( from );
        	}
        
        }
        { 	// Relational
        	using namespace checking::constants;
            using namespace checking::relational_op;
            do_check(
            	anon( a )( b )( c )( d )( e )( f )( g )( h )
            );
        }

    }

}// xxx_anon
}// xxx_anon
}// test_assign_v2
