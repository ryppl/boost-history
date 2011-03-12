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
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <boost/range/begin.hpp>
#include <boost/range/iterator.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modifier/standard.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <libs/assign/v2/test/put/pipe/modifier/standard.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_standard{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;

        // ------------------------------ WARNING ---------------------------- //
        // Don't misconstrue the commands in the tests below as *necessary* to //
        // obtain particular implementation. Most of the time the default is   //
        // already set at that invoked with operator%                            //
        // ------------------------------------------------------------------- //

        {
            //[pipe_modifier_push_front
            std::deque<int> powers; int front = (
                powers | ( as2::_put % as2::_push_front )( 16 )( 8 )( 4 )( 2 )( 1 )
            ).front();

            BOOST_ASSIGN_V2_CHECK( front == 1  );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 16 );
            //]
        }
        {
            //[pipe_modifier_push
            typedef int int_; std::queue<int_> fifo; int_ front = ( 
                fifo | ( as2::_put % as2::_push )( 72 )( 31 )( 48 ) 
            ).front();
                
            BOOST_ASSIGN_V2_CHECK( front == 72 );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == 48 );
            //]
        }
        {
            //[pipe_modifier_insert
            typedef std::set<double> doubles_; doubles_ sqrt2;
            range_iterator<doubles_>::type lower = ( 
                sqrt2 | ( as2::_put % as2::_insert )( 1.414 )( 1.41421 )( 1.41 )( 1.4142 ) 
            ).lower_bound( 1.41 );
        
            BOOST_ASSIGN_V2_CHECK( lower == begin( sqrt2 ) );
            BOOST_ASSIGN_V2_CHECK( sqrt2.upper_bound( 1.41421 ) == end( sqrt2 ) );
            //]
        }
    }

}// xxx_standard
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2