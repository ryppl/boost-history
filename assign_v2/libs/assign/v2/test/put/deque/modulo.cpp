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
#include <boost/typeof/typeof.hpp>
#include <boost/range/size.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/std/push_front.hpp>
#include <boost/assign/v2/put/ext.hpp>

#include <libs/assign/v2/test/put/deque/modulo.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_deque{
namespace xxx_modulo{

    // Tests interaction between different functionalities

    void test(){

        namespace as2 = boost::assign::v2;

        {
            //[put_deque_modulo_front
            BOOST_AUTO(
                cont,  (
                    as2::deque<int>( as2::_nil ) % as2::_push_front
                )( -1 )( 0 )( 1 )
            );
            BOOST_ASSIGN_V2_CHECK( cont.front() == 1 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == -1 );
            //]
            BOOST_ASSIGN_V2_CHECK( boost::size( cont ) == 3 );
        }
        {
            //[put_deque_modulo_repeat
            BOOST_AUTO(
                cont,  (
                    as2::deque<int>( as2::_nil ) % ( as2::_repeat = 2 )
                )( -1 )( 0 )( 1 )
            );
            BOOST_ASSIGN_V2_CHECK( boost::size( cont ) == 6 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont.front() == -1 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == 1 );
        }
    }

}// xxx_modulo
}// xxx_deque
}// xxx_put
}// test_assign_v2
