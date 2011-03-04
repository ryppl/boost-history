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
#include <boost/typeof/typeof.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/std/push_front.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/deque/functor.hpp>
#include <libs/assign/v2/test/put/std.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modulo{
namespace xxx_std{

    void test()
    {
        namespace as2 = boost::assign::v2;

        /* Only have to check those that are in /std which are not in
        /deduce/dependee.hpp */

        {
            //[put_std_push_front
            typedef int T; T x = 1, y = 2, z = 0; std::deque<T> cont;
            ( as2::put( cont ) % as2::_push_front )( z )( y )( x );
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[2] == z );
            //]
        }
        {
            typedef int T; T x = 1, y = 2, z = 0;
            //[put_deque_std_front
            BOOST_AUTO(
                cont,
                (
                    as2::deque<T>( as2::_nil ) % as2::_push_front
                )( x )( y )( z )
            );
            BOOST_ASSIGN_V2_CHECK( cont[0] == z );
            BOOST_ASSIGN_V2_CHECK( cont[2] == x );
            //]
        }

    }

}// xxx_std
}// xxx_modulo
}// xxx_put
}// test_assign_v2
