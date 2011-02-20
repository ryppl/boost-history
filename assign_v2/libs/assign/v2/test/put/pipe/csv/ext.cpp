//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <map>
#include <deque>
#include <string>

#include <boost/lambda/lambda.hpp>

#include <boost/assign/v2/put/pipe/csv.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/ext/lookup.hpp>
#include <boost/assign/v2/put/ext/repeat.hpp>
#include <boost/assign/v2/put/ext/iterate.hpp>
#include <libs/assign/v2/test/put/pipe/csv/ext.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_csv{
namespace xxx_ext{

    void test(){

        // This test is also about string literals

        namespace as2 = boost::assign::v2;
        namespace bl = boost::lambda;
        {
            //[csv_lookup
            std::map<std::string, int> cont;
            cont["jan"] = 29; cont["feb"] = 26; cont["mar"] = 29;
            cont | (
                as2::_csv_put % ( as2::_lookup = ( bl::_1 +=2 ) )
            )( "jan", "feb", "mar" );
            BOOST_ASSIGN_V2_CHECK( cont["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( cont["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( cont["mar"] == 31 );
            //]
        }
        {
            //[csv_repeat
            typedef int T; std::deque<T> cont; T x = 1, y = 2, z = 0;
            cont | ( as2::_csv_put % ( as2::_repeat = 2  ) )( x, y, z );
            BOOST_ASSIGN_V2_CHECK( cont.size() == 6 );
            //]
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[1] == x );
            BOOST_ASSIGN_V2_CHECK( cont[2] == y );
            BOOST_ASSIGN_V2_CHECK( cont[3] == y );
            BOOST_ASSIGN_V2_CHECK( cont[4] == z );
            BOOST_ASSIGN_V2_CHECK( cont[5] == z );
        }
        {
            //[csv_iterate
            typedef int T; T x = 1, y = 2, z = 0;
            boost::array<T, 3> cont; cont[0] = x;
            cont | (as2::_csv_put % ( as2::_iterate = 1  ) )( y, z );
            BOOST_ASSIGN_V2_CHECK( cont[0] == x );
            BOOST_ASSIGN_V2_CHECK( cont[1] == y );
            BOOST_ASSIGN_V2_CHECK( cont[2] == z );
            //]
        }
    }

}// xxx_ext
}// xxx_csv
}// xxx_pipe
}// xxx_put
}// xxx_test_assign
