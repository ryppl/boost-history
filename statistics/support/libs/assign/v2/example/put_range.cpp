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
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/constants.h>
#include <libs/assign/v2/example/put_range.h>

namespace example_assign_v2{
namespace xxx_put_range{

	void run(std::ostream& os)
    {
		using namespace boost::assign::v2;
        {
        	//[put_range_queue
            typedef std::vector<T> from_; from_ from( 3 );
            from[0] = x; from[1] = y; from[2] = z;
            typedef std::queue<T> to_; // FIFO
            to_ to;
            using namespace adaptor;
            BOOST_ASSERT( ( to | _put_range( from ) ).front() == x );
            BOOST_ASSERT( to.back() == z );
            //]
        }
    }

}// xxx_put_range
}// example_assign_v2
