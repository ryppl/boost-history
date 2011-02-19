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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/std/push_front.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <libs/assign/v2/test/put/std/functor.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_std{
namespace xxx_functor{

    void test()
    {
        namespace as2 = boost::assign::v2;

        /* Only have to check those that are in /std which are not in
        /deduce/dependee.hpp */

        {
            //[std_deque
            typedef int T; T x = 0, y = 2, z = 1;
            std::deque<T> seq;
            ( as2::put( seq ) % as2::_push_front )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( seq[0] == z );
            BOOST_ASSIGN_V2_CHECK( seq[2] == x );
            //]
        }
    }

}// xxx_functor
}// xxx_std
}// xxx_put
}// test_assign_v2
