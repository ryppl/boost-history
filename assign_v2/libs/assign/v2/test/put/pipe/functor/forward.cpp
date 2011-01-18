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
#include <vector>
#include <string>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/ref/list_tuple/container.hpp>
#include <boost/assign/v2/put/sub.hpp>
#include <boost/assign/v2/put/pipe/functor/forward.hpp>
#include <libs/assign/v2/test/put/pipe/functor/forward.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_forward{

	void test()
	{

        using namespace boost::assign::v2;
        int a1, b1, c1, d1;//, e1, f1, g1, h1;
        {
            using namespace checking::constants;
            a1 = a; b1 = b; c1 = c; d1 = d;
        }
        using namespace put_pipe_aux;
        {
            std::vector<int> cont;
	        BOOST_AUTO( lhs, ( put( cont ) ) );
	        forward(
    	        lhs,
    	        ref::list_tuple( a1 )( b1 )( c1 )( d1 )()
    	    );
        	BOOST_ASSIGN_V2_CHECK( cont[0] == a1 );
        	BOOST_ASSIGN_V2_CHECK( cont[1] == b1 );
        	BOOST_ASSIGN_V2_CHECK( cont[2] == c1 );
        	BOOST_ASSIGN_V2_CHECK( cont[3] == d1 );
        	BOOST_ASSIGN_V2_CHECK( cont.size() == 5 );
		}
        {
        	std::deque<const char*> cont;
	        BOOST_AUTO( lhs, put( cont ) );
	        forward( lhs, ref::list_tuple( "x" ) );
	        typedef std::string str_;
        	BOOST_ASSIGN_V2_CHECK( str_( cont[0] ) == "x" );
        }
	}

}// xxx_forward
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2
