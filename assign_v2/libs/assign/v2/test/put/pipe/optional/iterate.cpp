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

#include <boost/assign/v2/put/pipe/csv_put.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
// Options come next
#include <boost/assign/v2/optional/iterate.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/put/pipe/optional/iterate.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_optional{
namespace xxx_iterate{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        
        {
            //[test_put_pipe_modifier_iterate
            typedef int T; boost::array<T, 4> powers; powers[0] = 1; powers[1] = 10;

            int i = 2; 
            using namespace lambda;
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
}// xxx_optional
}// xxx_pipe
}// xxx_put
}// test_assign_v2
