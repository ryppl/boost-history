//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>

#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modifier/iterate.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <libs/assign/v2/test/put/pipe/modifier/iterate.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_iterate{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
        
        	// TODO construct arg_ with phoenix and perhaps skip
            //[iterate
            //typedef as2::functor_aux::post_increment<> arg_;
            //typedef int T; boost::array<T, 4> powers; powers[0] = 1; powers[1] = 10;
            //T front = ( 
            //	powers | ( as2::_put % ( as2::_iterate = arg_( 2 ) ) )( 100 )( 1000 )
            //).front();

            //BOOST_ASSIGN_V2_CHECK( front == 1 );
            //BOOST_ASSIGN_V2_CHECK( powers[1] == 10 );
            //BOOST_ASSIGN_V2_CHECK( powers[2] == 100 );
            //BOOST_ASSIGN_V2_CHECK( powers[3] == 1000 );
            //]
        }
    }

}// xxx_iterate
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2
