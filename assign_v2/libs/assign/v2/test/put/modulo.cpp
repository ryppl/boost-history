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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/std/push_front.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <boost/lambda/lambda.hpp>
#include <libs/assign/v2/test/put/modulo.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_modulo{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
            //[modulo_fun
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<int> cont;
            (
                as2::put( cont ) % ( as2::_fun = ( lambda::_1 + 1 ) )
            )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == ( x + 1 ) );
            BOOST_ASSIGN_V2_CHECK( cont.back() == ( z + 1 ) );
            //]
        }
        {
            //[modulo_push_front
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<int> cont;
            ( as2::put( cont ) % ( as2::_modifier
                = as2::modifier_tag::push_front() ) )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == z );
            BOOST_ASSIGN_V2_CHECK( cont.back() == x );
            //]
        }
    }

}// xxx_modulo
}// xxx_put
}// test_assign_v2
