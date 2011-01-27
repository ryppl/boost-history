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
#include <boost/assign/v2/put/pipe/functor.hpp>

#include <libs/assign/v2/test/put/pipe/functor/container.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_container{

    void test(){
        namespace as2 = boost::assign::v2;
        {	
            //[pipe_array	
            typedef int T;
            T x = 1, y = 2, z = 3;
            boost::array<T, 3> ar;
            ar | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( ar[0] == x );
            BOOST_ASSIGN_V2_CHECK( ar[2] == z );
            //]
        }
        {
        	//[pipe_map
            std::map<std::string, int> assoc;
            assoc | as2::_put( "jan", 31 )( "feb", 28 )( "mar", 31 );
            BOOST_ASSIGN_V2_CHECK( assoc["feb"] == 28 );
            //]
        }
		{
            //[pipe_set
            typedef std::string T;
            std::set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            assoc | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
		}        		    
        {
            //[pipe_deque
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<T> seq;
            seq | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[pipe_list
            typedef int T; T x = 1, y = 2, z = 0;
            std::list<T> seq;
            seq | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[pipe_vector
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<T> seq;
            seq | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq.front() == x );
            BOOST_ASSIGN_V2_CHECK( seq.back() == z );
            //]
        }
        {
            //[pipe_queue
            typedef int T; T x = 8, y = 7, z = 6;
            std::queue<T> fifo;
            fifo | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( fifo.front() == x );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == z );
            //]
		}
        {
            //[pipe_stack
            typedef int T; T x = 8, y = 7, z = 4;
            std::stack<T> lifo;
            lifo | as2::_put( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( lifo.top() == z ); lifo.pop();
            BOOST_ASSIGN_V2_CHECK( lifo.top() == y );
            //]
		}
    }// test()

}// xxx_container
}// xxx_functor  
}// xxx_pipe
}// xxx_put
}// xxx_test_assign
