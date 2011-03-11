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
#include <boost/assign/v2/put/modifier/push_front.hpp>
#include <boost/assign/v2/put/modifier/repeat.hpp>
#include <boost/assign/v2/put/container/put.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <libs/assign/v2/test/put/modifier/repeat.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modifier{
namespace xxx_repeat{

    void test()
    {

		namespace as2 = boost::assign::v2;
        {
            //[repeat_simple
            std::vector<int> cont;
            ( as2::put( cont ) % ( as2::_repeat = 2  ) )( 72 )( 31 )( 48 );

            BOOST_ASSIGN_V2_CHECK( cont.size() == 6 );
            BOOST_ASSIGN_V2_CHECK( cont.front() == 72 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == 48 );
            //]
        }
        {
            //[repeat_compose_deque
            BOOST_AUTO(
                cont, (
                    /*<<Notice that `_repeat` can be combined with another modifier, `_push_front`, in this case>>*/as2::deque<int>( as2::_nil ) % as2::_push_front % ( as2::_repeat = 2 )
                )( 72 )( 31 )( 48 )
            );
            BOOST_ASSIGN_V2_CHECK( boost::size( cont ) == 6 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont.front() == 48 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == 72 );
        }
	}


}// xxx_repeat
}// xxx_modifier
}// xxx_put
}// test_assign_v2
