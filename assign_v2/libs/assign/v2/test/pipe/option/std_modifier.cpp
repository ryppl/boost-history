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
#include <boost/assign/v2/pipe/csv_put.hpp>
#include <boost/assign/v2/deque.hpp>
// Options come next
#include <boost/assign/v2/option/modifier/std.hpp>
#include <libs/assign/v2/test/pipe/option/std_modifier.h>

namespace test_assign_v2{
namespace xxx_pipe{
namespace xxx_option{
namespace xxx_std_modifier{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;

        // ------------------------------ WARNING ---------------------------- //
        // Don't misconstrue the commands in the tests below as *necessary* to //
        // obtain particular implementation. Most of the time the default is   //
        // already set at that invoked with operator%                          //
        // ------------------------------------------------------------------- //
        
        // MSVC REMINDER : fully qualify boost::begin boost::end - error C2668

        {
            //[test_pipe_option_push_front
            std::deque<int> powers; int front = (
                powers | ( as2::_csv_put % as2::_push_front )( 16, 8, 4, 2, 1 )
            ).front();

            BOOST_ASSIGN_V2_CHECK( front == 1  );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 16 );
            //]
        }
        {
            //[test_pipe_option_push
            typedef int int_; std::queue<int_> fifo; int_ front = ( 
                fifo | ( as2::_csv_put % as2::_push )( 72, 31, 48 ) 
            ).front();
                
            BOOST_ASSIGN_V2_CHECK( front == 72 );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == 48 );
            //]
        }
        {
            //[test_pipe_option_insert
            typedef std::set<double> doubles_; doubles_ sqrt2;
            range_iterator<doubles_>::type lower = ( 
                sqrt2 | ( as2::_csv_put % as2::_insert )( 1.414, 1.41421, 1.41, 1.4142 ) 
            ).lower_bound( 1.41 );
        
            BOOST_ASSIGN_V2_CHECK( lower == boost::begin( sqrt2 ) );
            BOOST_ASSIGN_V2_CHECK( sqrt2.upper_bound( 1.41421 ) == boost::end( sqrt2 ) );
            //]
        }
    }

}// xxx_std_modifier
}// xxx_option
}// xxx_pipe
}// test_assign_v2