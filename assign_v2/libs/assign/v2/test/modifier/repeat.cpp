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
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/modifier/push_front.hpp>
#include <boost/assign/v2/modifier/repeat.hpp>
#include <boost/assign/v2/value/put.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/value/modifier/repeat.h>

namespace test_assign_v2{
namespace xxx_value{
namespace xxx_modifier{
namespace xxx_repeat{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_value_modifier_repeat_simple
            std::vector<int> cont;
            ( as2::put( cont ) % ( as2::_repeat = 2  ) )( 72 )( 31 )( 48 );

            BOOST_ASSIGN_V2_CHECK( range::equal( cont, as2::csv_deque( 72, 72, 31, 31, 48, 48 ) ) );
            //]
        }
        {
            //[test_value_modifier_repeat_compose
            BOOST_AUTO(
                cont, (
                    as2::deque<int>( as2::_nil ) % as2::_push_front % ( as2::_repeat = 2 )
                )( 72 )( 31 )( 48 )
            );
            
            BOOST_ASSIGN_V2_CHECK( range::equal( cont, as2::csv_deque( 48, 48, 31, 31, 72, 72 ) ) );
            //]
        }
    }


}// xxx_repeat
}// xxx_modifier
}// xxx_value
}// test_assign_v2
