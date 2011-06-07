//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <bitset>
#include <deque>
#include <map>
#include <string>
#include <vector>
#include <boost/assign/v2/include/csv_deque_basic.hpp>
#include <boost/assign/v2/include/csv_put_ext.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/traits.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <tuple>
#else
#include <boost/tuple/tuple.hpp>
#endif
#include <libs/assign/v2/test/put/put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_csv_put_ext{

    void test(){

        namespace as2 = boost::assign::v2;
        // SEQUENCE
        {
            //[test_csv_put_ext1
            typedef std::string str_;
            typedef std::bitset<3> number;
            std::vector<number> range_3bit;

            // The significance of this example comes from:
            // Expression                               Valid
            // number( "011" )                          No
            // number( str_( "011" ) )                  Yes
            // range_3bit.push_back( str_( "011" ) )    No

            as2::csv_put<1, /*<<Required>>*/str_>(
                range_3bit
                , "000", "001", "010", "011", "100", "101", "110", "111"
            );

            for(unsigned i = 0; i < range_3bit.size(); i++)
            {
                BOOST_ASSIGN_V2_CHECK( range_3bit[i].to_ulong() == i );
            }
            //]
        }
        {
            //[test_csv_put_ext2
            typedef const char state_ [3]; typedef int code_;
            state_ ct = "CT", nj = "NJ", ny = "NY";

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
            typedef std::tuple<state_/*<<Notice the reference>>*/&,  code_> data_;
//<-
#else
            typedef boost::tuple<state_&,  code_> data_;
#endif
//->
            std::deque< data_ > region;

            as2::csv_put<2>(
                region,
                ny, 212, ny, 718, ny, 516, ny, 914,
                nj, 210, nj, 908, nj, 609,
                ct, 203
            );

//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
            BOOST_ASSIGN_V2_CHECK( std::get<0>( region.front() ) == ny );
            BOOST_ASSIGN_V2_CHECK( std::get<1>( region.back() ) == 203 );
            BOOST_ASSIGN_V2_CHECK( std::get<1>( region.front() ) == 212 );
            BOOST_ASSIGN_V2_CHECK( std::get<0>( region.back() ) == ct );
//<-
#endif
//->
            //]
#if! BOOST_ASSIGN_V2_ENABLE_CPP0X
            BOOST_ASSIGN_V2_CHECK( boost::get<0>( region.front() ) == ny );
            BOOST_ASSIGN_V2_CHECK( boost::get<1>( region.back() ) == 203 );
            BOOST_ASSIGN_V2_CHECK( boost::get<1>( region.front() ) == 212 );
            BOOST_ASSIGN_V2_CHECK( boost::get<0>( region.back() ) == ct );
#endif
        }
        // MAP
        {
            //[test_csv_put_ext3
            std::map<std::string, int> q1;
            as2::csv_put<2>( q1, "jan", 31, "feb", 28, "mar", 31 );

            BOOST_ASSIGN_V2_CHECK( q1["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( q1["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( q1["mar"] == 31 );
            //]
        }

        // Pointer-containers

        {
            //[test_csv_put_ext4
            typedef std::string month_; typedef int days_;
            boost::ptr_map<month_, days_> q1;

            as2::csv_put<2>( q1, "jan", 31, "feb", 28, "mar", 31 );

            BOOST_ASSIGN_V2_CHECK( q1["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( q1["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( q1["mar"] == 31 );
            //]
        }

    }// test()

}// xxx_csv_put_ext
}// xxx_put
}// xxx_test_assign
