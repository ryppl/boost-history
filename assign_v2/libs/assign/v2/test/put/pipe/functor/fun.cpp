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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/lambda/lambda.hpp>
#include <libs/assign/v2/test/put/pipe/functor/fun.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_functor{
namespace xxx_fun{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace bl = boost::lambda;
        {
            //[fun
            typedef int T; T x = 1, y = 2, z = 0;
            std::vector<int> cont;
            cont | ( 
                as2::_put % ( as2::_fun = ( bl::_1 + 1 ) ) 
            )( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == ( x + 1 ) );
            BOOST_ASSIGN_V2_CHECK( cont.back() == ( z + 1 ) );
            //]
        }
    }

}// xxx_fun
}// xxx_functor
}// xxx_pipe
}// xxx_put
}// test_assign_v2
