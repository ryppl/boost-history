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
#include <boost/assign/v2/pipe/csv_put.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/modifier/repeat.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/pipe/option/repeat.h>

namespace test_assign_v2{
namespace xxx_pipe{
namespace xxx_option{
namespace xxx_repeat{

    void test()
    {

        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_pipe_option_repeat
            std::vector<int> cont;
            BOOST_ASSIGN_V2_CHECK(
                range::equal(
                    cont | ( as2::_csv_put % ( as2::_repeat = 2  ) )( 1, 10 ,100 ),
                    as2::csv_deque<int>( 1, 1, 10, 10, 100, 100 )
                )
            );
            //]
        }
    }


}// xxx_repeat
}// xxx_option
}// xxx_pipe
}// test_assign_v2
