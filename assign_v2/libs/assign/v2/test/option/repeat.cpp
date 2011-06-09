//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/include/csv_basic.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/include/do_put.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/assign/v2/option/modifier/repeat.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/option/repeat.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_repeat{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            //[test_repeat1
            std::vector<int> cont;

            BOOST_AUTO( _modifier, ( as2::_repeat = 2 ) );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    cont | (
                        as2::_do_put % _modifier
                    ).for_each( as2::csv_deque(1, 10, 100) ),
                    as2::csv_deque( 1, 1, 10, 10, 100, 100 )
                )
            );
            //]
        }
        {
            //[test_repeat2
            BOOST_AUTO( _modifier, ( as2::_repeat = 2 ) );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    csv(
                        as2::deque<int, as2::push_front_>( as2::_nil) % _modifier
                        , 1, 10, 100, 1000
                    ),
                    as2::csv_deque( 1000, 1000, 100, 100, 10, 10, 1, 1 )
                )
            );
            //]
        }
    }


}// xxx_repeat
}// xxx_option
}// test_assign_v2
