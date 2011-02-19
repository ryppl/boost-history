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
#include <stack>
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/pipe/range.hpp>

#include <libs/assign/v2/test/put/pipe/range.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_range{

	void test()
    {
        namespace as2 = boost::assign::v2;
        
        //[range_var
    	typedef int T; T x = 1, y = 2, z = 0;
        std::vector<T> r( 3 ); r[0] = x; r[1] = y; r[2] = z;
        //]       
        
        // Forwards to put()
		{
        	//[range_array
        	boost::array<T, 3> cont; cont | as2::_put_range( r );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
		{
        	//[range_stack
    		typedef std::stack<T> lifo_;
    		BOOST_ASSIGN_V2_CHECK( 
            	( 
                	::boost::type<lifo_>() | as2::_put_range( r ) 
                ).top() == z 
            );
            //]
        }

        // Calls C(b, e)
		{
        	//[range_deque
        	std::deque<T> cont; cont | as2::_put_range( r );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
    }

}// xxx_range
}// xxx_pipe
}// xxx_put
}// test_assign_v2
