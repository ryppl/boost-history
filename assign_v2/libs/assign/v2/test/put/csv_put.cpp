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
#include <list>
#include <queue>
#include <set>
#include <string>
#include <vector>

#define BOOST_ASSIGN_V2_LIMIT_CSV_ARITY 24

#include <boost/assign/v2/include/csv_deque.hpp>
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/traits.hpp>
#include <boost/multi_array.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>
#include <boost/variant.hpp>
#include <libs/assign/v2/test/put/put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_csv_put{

    void test(){

        namespace as2 = boost::assign::v2;
        // STL
        {
            //[test_csv_put1
            std::list<int> seq; as2::csv_put( seq, 1, 2, 3, 4, 5 );
            std::queue<int> adapter; as2::csv_put( adapter, 1, 2, 3, 4, 5 );
            boost::array<int,  5> array; as2::csv_put( array, 1, 2, 3, 4, 5 );
            std::set<int> assoc; as2::csv_put( assoc, 1, 2, 3, 4, 5 );
            //]

            std::vector<int> benchmark( 5 ); benchmark[0] = 1;
            benchmark[1] = 2; benchmark[2] = 3;
            benchmark[3] = 4; benchmark[4] = 5;

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, seq ) );
            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, array ) );
            BOOST_ASSIGN_V2_CHECK( adapter.front() == 1 );
            BOOST_ASSIGN_V2_CHECK( adapter.back() == 5 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( 1 ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( 5 ) == 1 );
        }
        // SEQUENCE
        {
            //[test_csv_put2
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
            //[test_csv_put3
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
        //SET
        {
            //[test_csv_put4
            typedef std::string word_;
            typedef boost::unordered_set<word_> C;
            C set, benchmark;

            as2::csv_put( set, "foo", "bar", "baz" );

            benchmark.insert( "foo" );
            benchmark.insert( "bar" );
            benchmark.insert( "baz" );
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( set, benchmark )
            );
            //]
        }
        // MAP
        {
            //[test_csv_put5
            std::map<std::string, int> q1;
            as2::csv_put<2>( q1, "jan", 31, "feb", 28, "mar", 31 );

            BOOST_ASSIGN_V2_CHECK( q1["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( q1["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( q1["mar"] == 31 );
            //]
        }
        // MULTI-ARRAY
        {
            //[test_csv_put6
            typedef boost::multi_array<int, 3> array3_;
            typedef array3_::size_type size_;

            typedef const size_ dim_;
            dim_ dim1 = 2, dim2 = 3, dim3 = 4;

            array3_ array3( boost::extents[dim1][dim2][dim3] );

            as2::csv_put(
                array3,
                 0,  1,  2,  3,
                 4,  5,  6,  7,
                 8,  9, 10, 11,

                12, 13, 14, 15,
                16, 17, 18, 19,
                20, 21, 22, 23
            );

            int i = 0;
            for( size_ i1 = 0; i1 < dim1; i1++ )
            {
                for( size_ i2 = 0; i2 < dim2; i2++ )
                {
                    for( size_ i3 = 0; i3 < dim3; i3++ )
                    {
                        BOOST_ASSIGN_V2_CHECK( array3[ i1 ][ i2 ][ i3 ] == i++ );
                    }
                }
            }

            //]

            typedef boost::multi_array_types::index_range range_;
            array3_::array_view<2>::type view
                = array3[ boost::indices[1][range_(0,2)][range_(1,3)] ];

            as2::csv_put(
                view,
                99, 98,
                97, 96
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    as2::csv_deque(
                        view[0][0], view[0][1],
                        view[1][0], view[1][1]
                    ),
                    as2::csv_deque(
                        99, 98,
                        97, 96
                    )
                )
            );

        }
    }// test()

}// xxx_csv_put
}// xxx_put
}// xxx_test_assign
