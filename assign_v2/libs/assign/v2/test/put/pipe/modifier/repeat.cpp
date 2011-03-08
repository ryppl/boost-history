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
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modifier/repeat.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <libs/assign/v2/test/put/pipe/modifier/repeat.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_repeat{

    void test()
    {

		namespace as2 = boost::assign::v2;
        {
            //[repeat
            std::vector<int> cont;
            int front = (
            	cont | ( as2::_put % ( as2::_repeat = 2  ) )( 72 )( 31 )( 48 )
            ).front();

            BOOST_ASSIGN_V2_CHECK( cont.size() == 6 );
            BOOST_ASSIGN_V2_CHECK( front == 72 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == 48 );
            //]
        }
	}


}// xxx_repeat
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2
