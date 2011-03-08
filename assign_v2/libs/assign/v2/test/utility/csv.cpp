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
#include <boost/typeof/typeof.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/csv.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/modifier/push_front.hpp>
#include <libs/assign/v2/test/utility/csv.h>

namespace test_assign_v2{
namespace xxx_utiliy{
namespace xxx_csv{

    // Tests interaction between different functionalities

    void test(){

        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
            //[csv_put
            std::deque<int> powers;
            as2::csv( as2::put( powers ), 16, 8, 4, 2, 1 );

            BOOST_ASSIGN_V2_CHECK( powers[0] == 6 );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 1 );
            //]
        }
        {
            //[csv_put_modulo
            std::deque<int> powers;
            as2::csv( as2::put( powers ) % as2::_push_front, 16, 8, 4, 2, 1 );

            BOOST_ASSIGN_V2_CHECK( powers[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 16 );
            //]
        }
        {
            //[csv_modulo
            BOOST_AUTO(
            	powers,
            	as2::csv( as2::deque<int>( as2::_nil ), 16, 8, 4, 2, 1 )
            );

            BOOST_ASSIGN_V2_CHECK( powers[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 16 );
            //]
        }
        {
            //[csv_deque_modulo
            BOOST_AUTO(
            	powers,
            	as2::csv( as2::deque<int>( as2::_nil ) % as2::_push_front, 16, 8, 4, 2, 1 )
            );

            BOOST_ASSIGN_V2_CHECK( powers[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 16 );
            //]
            //]
        }
    }

}// xxx_csv
}// xxx_utility
}// xxx_test_assign
