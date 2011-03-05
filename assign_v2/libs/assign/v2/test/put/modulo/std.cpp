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
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <boost/typeof/typeof.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/std.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/deque/functor.hpp>
#include <libs/assign/v2/test/put/modulo/std.h>

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
            //[put_std_push
            typedef int T; T x = 8, y = 7, z = 4;
            typedef std::queue<T> cont_;
            cont_ fifo; ( as2::put( fifo ) % as2::_push )( x )( y )( z );
                
            BOOST_ASSIGN_V2_CHECK( fifo.front() == x );
            BOOST_ASSIGN_V2_CHECK( fifo.back() == z );

            //]
        }
        {
            //[put_std_insert
            typedef std::string T; std::set<T> cont;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            ( as2::put( cont ) % as2::_insert )( x )( y )( z );
        
            BOOST_ASSIGN_V2_CHECK( cont.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( cont.count( z ) == 1 );
            //]
        }
        {
            // Lunar eclipse years falling on winter solstice
            //[put_std_push_back
            typedef int T; T x = 1638, y = 2010, z = 2094; 
            std::list<T> cont; ( as2::put( cont ) % as2::_push_back )( x )( y )( z );
                
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
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
