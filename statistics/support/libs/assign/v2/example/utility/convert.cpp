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
#include <stack>
#include <boost/array.hpp>
#include <boost/assign/v2/misc/convert.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/misc/convert.h>

namespace example_assign_v2{
namespace xxx_misc{
namespace xxx_convert{

    void run(std::ostream& os)
    {
    	{
            //[convert_stack
            std::vector<int> v( 3 ); v[0] = -1; v[1] = 0; v[2] = 1;
            std::stack<int> lifo = as2::converter( v ); 
            assert( lifo.top() == 1 );
            //]
        }
        {
            //[convert_array
            std::vector<int> v( 3 ); v[0] = -1; v[1] = 99; v[2] = 1;
            typedef boost::array<int, 3> ar_;
            assert( ( as2::converter( v ).type<ar_>() )[1] == 99 );
            //]
        }
    }

}// xxx_convert
}// xxx_misc
}// example_assign_v2
