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
#include <boost/lambda/lambda.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/std/push_front.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>
#include <libs/assign/v2/test/put/pipe/functor/std.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_std{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
            //[put_pipe_functor_push_front
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<int> cont;
            cont | ( as2::_put %  as2::_push_front )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == z );
            BOOST_ASSIGN_V2_CHECK( cont.back() == x );
            //]
        }
        {
            //[put_pipe_csv_push_front
            typedef int T; T x = 1, y = 2, z = 0;
            std::deque<int> cont;
            cont | ( as2::_csv_put %  as2::_push_front )( x, y, z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == z );
            BOOST_ASSIGN_V2_CHECK( cont.back() == x );
            //]
        }
    }

}// xxx_std
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2
