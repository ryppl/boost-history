//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <queue>
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2/put/pipe/range.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/put/range.h>

namespace example_assign_v2{
namespace xxx_put{
namespace xxx_range{

    void run(std::ostream& os)
    {
        {
            //[put_range_queue
            std::vector<int> v( 3 ); v[0] = -1; v[1] = 0; v[2] = 1;
            std::queue<int> fifo; 
            assert( ( fifo | as2::_put_range( v ) ).front() == -1 );
            assert( fifo.back() == 1 );
            //]
        }
		{
            //[put_range_array
    		typedef boost::array<int, 2> cont_;
    		std::vector<int> v(2); v[0] = 1; v[1] = -99; 
    		assert(
    			(boost::type<cont_>() | as2::_put_range( v ) )[1] == -99
    		);
            //]
        }
    }

}// xxx_range
}// xxx_put
}// example_assign_v2
