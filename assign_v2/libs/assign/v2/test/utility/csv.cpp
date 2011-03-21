//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/csv.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/modifier/push_front.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/utility/csv.h>

namespace test_assign_v2{
namespace xxx_utility{
namespace xxx_csv{

    void test(){

        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_utility_csv_put
            std::deque<int> cont;
            as2::csv( as2::put( cont ), 72, 31, 48 );

            BOOST_ASSIGN_V2_CHECK( range::equal( cont, as2::csv_deque( 72, 31, 48 ) ) );
            //]
        }
        {
            //[test_utility_csv_put_modulo
            std::deque<int> cont;
            as2::csv( as2::put( cont ) % as2::_push_front, 72, 31, 48 );

            BOOST_ASSIGN_V2_CHECK( range::equal( cont, as2::csv_deque( 48, 31, 72 ) ) );
            //]
        }
        {
            //[test_utility_csv_deque_modulo
            BOOST_AUTO(
                cont,
                as2::csv( as2::deque<int>( as2::_nil ) % as2::_push_front, 72, 31, 48 )
            );

            BOOST_ASSIGN_V2_CHECK( range::equal( cont, as2::csv_deque( 48, 31, 72 ) ) );
            //]
        }
    }

}// xxx_csv
}// xxx_utility
}// xxx_test_assign
