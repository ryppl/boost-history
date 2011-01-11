//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <string>
#include <assert.h>
#include <boost/assign/v2/put/deque.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/foo.h>
#include <libs/assign/v2/example/put/deque.h>

namespace example_assign_v2{
namespace xxx_put{
namespace xxx_deque{

    void run(std::ostream& os)
    {
        os << "* xxx_deque " << std::endl;
        {
            //[csv_deque
            boost::for_each(
                as2::csv_deque<std::string>("x", "y", "z" ),
                os << bl::_1 << ' '
            ); // outputs x, y, z
            //]
            os << std::endl;
            {
				assert( as2::deque<Foo>().size() == 1 );
            }
            {
            	//[deque
            	int x = 0, a = 1;
            	boost::for_each(
                	as2::deque<Foo>( x )()( a, -1 ),
                	os << bl::_1 << ' '
            	);
            	// outputs (0,0)(NULL,0)(1,-1)
            	//]
	            os << std::endl;
            }
            {
            	//[deque_front
            	boost::for_each(
                    (
                        as2::deque<int>( as2::_nil ) % as2::_push_front
                    )( -1 )( 0 )( 1 ),
                    os << bl::_1 << ' '
            	);
            	// outputs 1 0 -1
            	//]
	            os << std::endl;
            }
        }
    }

}// xxx_deque
}// xxx_put
}// example_assign_v2
