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
#include <boost/assign/v2/option/list.hpp>
#include <boost/assign/v2/option/data_generator.hpp>
#include <boost/assign/v2/option/modifier/std.hpp>
#include <boost/assign/v2/put.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/static_assert.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/list.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_list{

    void test()
    {
        //xxx_csv_put::test();
        //xxx_option::test();
        using namespace boost::assign::v2;
        std::deque<int> cont;

        ( _list_option ).apply( put( cont ) )( 1 );
        BOOST_ASSIGN_V2_CHECK( cont[0] == 1 );
        using namespace boost::lambda;
        BOOST_AUTO( 
            apply, 
            ( 
                _list_option % _push_front % ( _data = ( _1 % 10 ) ) 
            ).apply( put( cont ) ) 
        );
        apply( 15 );
        BOOST_ASSIGN_V2_CHECK( cont[0] == 5 );
        BOOST_ASSIGN_V2_CHECK( cont[1] == 1 );

    }

}// xxx_list
}// xxx_option
}// xxx_test_assign

