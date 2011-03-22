//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <list>
#include <vector>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp>
// Options come next
#include <boost/assign/v2/option/data.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/put/pipe/option/data.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_option{
namespace xxx_fun{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_put_pipe_option_data_math
            int k = 1; std::list<int> factorials;
            using namespace lambda;
            
            BOOST_ASSIGN_V2_CHECK(
                range::equal(
                    factorials | ( 
                        as2::_csv_put % ( as2::_data = ( var(k) *= _1 ) ) 
                    )/*Equivalent to calling `factorials.push_back( k *= ++i )`*/( 1, 2, 3, 4, 5 ),
                    as2::csv_deque<int>( 1 )( 2 )( 6 )( 24 )( 120 )
                )
            );
            //]
        }
    }

}// xxx_fun
}// xxx_option
}// xxx_pipe
}// xxx_value
}// test_assign_v2
