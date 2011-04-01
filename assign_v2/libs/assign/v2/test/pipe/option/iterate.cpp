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

#include <boost/assign/v2/pipe/csv_put.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
// Options come next
#include <boost/assign/v2/option/modifier/iterate.hpp>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/pipe/option/iterate.h>

namespace test_assign_v2{
namespace xxx_pipe{
namespace xxx_option{
namespace xxx_iterate{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        
        {
            //[test_pipe_option_iterate
            typedef int T; 
            boost::array<T, 10> alternating; 
            alternating[0] = -1; alternating[1] = +1;
            alternating[2] = -2;

            int i = 3; 
            using namespace lambda;
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    alternating | ( 
                        as2::_csv_put % ( as2::_iterate = var( i )++ ) 
                    )( +2, -3, +3, -4, +4, -5, +5 ),
                    as2::csv_deque<T>( -1, +1, -2, +2, -3, +3, -4, +4, -5, +5 )    
                )
            );
            //]
        }
    }

}// xxx_iterate
}// xxx_option
}// xxx_pipe
}// test_assign_v2
