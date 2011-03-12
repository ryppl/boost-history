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
#include <boost/assign/v2/put/modifier/repeat.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/put/pipe/modifier/repeat.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_repeat{

    void test()
    {

        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[pipe_repeat
            std::vector<int> cont;
            BOOST_ASSIGN_V2_CHECK(
                range::equal(
                    cont | ( as2::_put % ( as2::_repeat = 2  ) )( 72 )( 31 )( 48 ),
                    as2::csv_deque<int>( 72, 72, 31, 31, 48, 48 )
                )
            );
            //]
        }
    }


}// xxx_repeat
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2
