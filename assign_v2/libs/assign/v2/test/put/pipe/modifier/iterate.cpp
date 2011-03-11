//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modifier/iterate.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <libs/assign/v2/test/put/pipe/modifier/iterate.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_iterate{

    void test()
    {
        using namespace boost;
        using namespace lambda;
        namespace as2 = assign::v2;
        
        {
        
            //[pipe_iterate
            typedef int T; boost::array<T, 4> powers; powers[0] = 1; powers[1] = 10;
            int i = 2; 
            BOOST_ASSIGN_V2_CHECK(
            	boost::range::equal(
            		powers | ( as2::_csv_put % ( as2::_iterate = var( i )++ ) )( 100, 1000 ),
                	as2::csv_deque<T>( 1, 10, 100, 1000 )    
                )
			);
            //]
        }
    }

}// xxx_iterate
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2
