//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <map>
#include <boost/assign/v2/include/do_csv_put.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <libs/assign/v2/test/put/do_csv_put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_do_csv_put{

    void test(){
        namespace as2 = boost::assign::v2;
// TODO Allow CPP03
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
        // MAP
        {
            //[test_do_csv_put1
            typedef std::string month_;
            typedef int days_;
            std::map<month_, days_> q1, benchmark;

            benchmark[ "jan" ] = 31;
            benchmark[ "feb" ] = 28;
            benchmark[ "mar" ] = 31;

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    benchmark,
                    q1 | as2::do_csv_put<2>( "jan", 31, "feb", 28, "mar", 31 )
                )
            );
            //]
        }
#endif
    }// test()

}// xxx_do_put
}// xxx_put
}// xxx_test_assign
