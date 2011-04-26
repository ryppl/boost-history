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
#include <boost/assign/v2/include/csv_deque.hpp>
#include <boost/assign/v2/option/list.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/list.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_list{

    void test()
    {
        namespace as2 = boost::assign::v2;
        
        {
            //[test_option_list
            std::deque<int> cont;
            ( as2::put( cont ) % as2::_option )( 4 )( 5 );
            
            BOOST_ASSIGN_V2_CHECK( cont[0] == 4 );
            BOOST_ASSIGN_V2_CHECK( cont[1] == 5 );

            BOOST_AUTO( 
                options, 
                as2::_push_front % ( as2::_data = ( boost::lambda::_1 % 10 ) ) 
            );
            ( as2::put( cont ) % options )( 13 )( 12 )( 11 );

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal( cont, as2::csv_deque( 1, 2, 3, 4, 5 ) ) 
            );
            //]
        }

    }
    
}// xxx_list
}// xxx_option
}// xxx_test_assign

