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
            std::vector<int> from( 3 );
            from[0] = -1; from[1] = 0; from[2] = 1;
            std::queue<int> to; // FIFO
            BOOST_ASSERT(
                (
                    to | as2::_put_range( from )
                ).front() == -1
            );
            BOOST_ASSERT( to.back() == 1 );
            //]
        }
    }

}// xxx_range
}// xxx_put
}// example_assign_v2
