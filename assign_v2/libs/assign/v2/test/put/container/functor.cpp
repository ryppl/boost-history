//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <string>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/put/container/functor.hpp>

#include <libs/assign/v2/test/put/container/functor.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_container{
namespace xxx_functor{

    void test(){
        namespace as2 = boost::assign::v2;
        {	
            //[array	
            typedef int T;
            T x = 1, y = 2, z = 3;
            boost::array<T, 3> ar;
            as2::put( ar )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( ar[0] == x );
            BOOST_ASSIGN_V2_CHECK( ar[2] == z );
            //]
        }
        {
        	//[map
            std::map<std::string, int> assoc;
            as2::put( assoc )( "jan", 31 )( "feb", 28 )( "mar", 31 );
            BOOST_ASSIGN_V2_CHECK( assoc["feb"] == 28 );
            //]
        }
		{
            //[set
            typedef std::string T;
            std::set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::put( assoc )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
		}        		    
        {
            //[deque
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<T> seq;
            as2::put( seq )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[list
            typedef int T; T x = 1, y = 2, z = 0;
            std::list<T> seq;
            ( as2::put( seq ) )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[vector
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<T> seq;
            as2::put( seq )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[queue
            typedef int T; T x = 8, y = 7, z = 6;
            std::queue<T> fifo;
            as2::put( fifo )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( fifo.front() == x );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == z );
            //]
		}
        {
            //[stack
            typedef int T; T x = 8, y = 7, z = 4;
            std::stack<T> lifo;
            as2::put( lifo )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == z ); lifo.pop();
            BOOST_ASSIGN_V2_CHECK( lifo.top() == y );
            //]
		}
    }// test()

}// xxx_functor  
}// xxx_container
}// xxx_put
}// xxx_test_assign
