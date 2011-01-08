//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <boost/array.hpp>
#include <boost/assign/v2/put/pipe/convert.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/constants.h>
#include <libs/assign/v2/example/put/convert.h>

namespace example_assign_v2{
namespace xxx_put{
namespace xxx_convert{

    void run(std::ostream& os)
    {
        {
            {
            	//[convert_stack
                boost::array<int, 3> from;
                typedef std::stack<int> to_; // LIFO
                from[0] = -1; from[1] = 0; from[2] = 1;
                BOOST_ASSERT(
                    (
                        from | as2::put_convert<to_>()
                    ).top() == 1
                );
                //]
            }
        }
    }

}// xxx_convert
}// xxx_put
}// example_assign_v2
