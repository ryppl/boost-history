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
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/constants.h>
#include <libs/assign/v2/example/ref_anon.h>

namespace example_assign_v2{
namespace xxx_conversion{

    void run(std::ostream& os)
    {
        using namespace boost::assign::v2;
        namespace ns_ref = ref::assign_copy;
        {
            {
            	//[convert_stack
                typedef boost::array<T, 3> from_;
                typedef std::stack<T> to_; // LIFO
                from_ from; from[0] = x; from[1] = y; from[2] = z;
                using namespace adaptor;
                BOOST_ASSERT(
                	( from | put_convert<to_>() ).top() == z 
                );
                //]
            }
        }
    }

}// xxx_conversion
}// example_assign_v2
