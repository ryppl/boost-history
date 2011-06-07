//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <map>
#include <vector>

#include <boost/assign/v2/include/csv.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/ptr_container/ptr_unordered_map.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>
#include <libs/assign/v2/test/put/put.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_put{

    void test(){

        namespace as2 = boost::assign::v2;
        // MAP
        {
            //[test_put1
            typedef std::string word_;
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            boost::unordered_map<int, word_> map;

            as2::put( map )( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux");

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }
        {
            //[test_put2
            typedef std::string month_; typedef int days_;
            typedef std::map<month_, days_> map_;
            typedef boost::tuple<month_ const, days_> pair_;
            std::deque<pair_> input;

            input.push_back( pair_("jan", 31) );
            input.push_back( pair_("feb", 28) );
            input.push_back( pair_("mar", 31) );

            map_ map; as2::put( map ).for_each<2>( input );

            BOOST_ASSIGN_V2_CHECK( map["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( map["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( map["mar"] == 31 );
            //]
        }
        // ARRAY
        {
            //[test_put3
            typedef double fp_; typedef std::vector<fp_> data_;
            data_ a( 3 ); a[0] = 0.71; a[1] = 0.63; a[2] = 0.85;
            data_ b( 4 ); b[0] = 0.61; b[1] = 0.69; b[2] = 0.92; b[3] = 0.55;
            boost::array<data_, 4> ragged;

            as2::put( ragged )
                ( boost::begin( a ), boost::end( a ) )
                ( b )
                ( 1, -99.99 )
                ( );

            BOOST_ASSIGN_V2_CHECK( ragged[0].size() == a.size() );
            BOOST_ASSIGN_V2_CHECK( ragged[3].size() == 0 );
            //]
            BOOST_ASSIGN_V2_CHECK( ragged[1].size() == b.size() );
            BOOST_ASSIGN_V2_CHECK( ragged[2].size() == 1 );
            fp_ eps = boost::numeric::bounds<fp_>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[0].front() - a.front() ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[0].back() - a.back() ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[1].front() - b.front() ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[1].back() - b.back() ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[2].front() + 99.99 ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[2].back() + 99.99 ) < eps );
        }

        // Pointer containers

        // MAP
        {
            //[test_put4
            typedef std::string word_;
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            boost::ptr_unordered_map<int, word_> map;

            as2::put( map )( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux" );

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }

    }// test()

}// xxx_put
}// xxx_put
}// xxx_test_assign
