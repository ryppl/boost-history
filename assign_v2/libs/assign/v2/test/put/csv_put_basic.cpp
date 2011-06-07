//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <list>
#include <queue>
#include <set>
#include <string>
#include <vector>

#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/ptr_container/ptr_unordered_set.hpp>

#define BOOST_ASSIGN_V2_LIMIT_CSV_ARITY 24

#include <boost/assign/v2/include/csv_deque_basic.hpp>
#include <boost/assign/v2/include/csv_put_basic.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/assign/v2/support/traits.hpp>
#include <boost/multi_array.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/unordered_set.hpp>
#include <libs/assign/v2/test/put/csv_put_basic.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_csv_put_basic{

    void test(){

        namespace as2 = boost::assign::v2;
        // STL
        {
            //[test_csv_put_basic1
            std::list<int> seq; as2::csv_put( seq, 1, 2, 3, 4, 5 );
            std::queue<int> adapter; as2::csv_put( adapter, 1, 2, 3, 4, 5 );
            boost::array<int, 5> array; as2::csv_put( array, 1, 2, 3, 4, 5 );
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
        //SET
        {
            //[test_csv_put_basic2
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
        // MULTI-ARRAY
        {
            //[test_csv_put_basic3
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

        // Pointer-containers

        {
            //[test_csv_put_basic4
            typedef int T;
            T x = 1, y = 2, z = 3; boost::ptr_array<T, 3> cont;
            /*<<Calls `assoc[i++] = new T( t )` for `i = 0` and `t`[^ = ] `x`, `y`, and `z`>>*/as2::csv_put( cont, x, y, z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // SEQUENCE
        {
            //[test_csv_put_basic5
            typedef int T; T x = 1, y = 2, z = 0; boost::ptr_deque<T> cont;
            /*<<Calls `assoc.push_back( new T( t ) )` for `t` [^=] `x`, `y`, and `z`>>*/as2::csv_put( cont, x, y, z );

            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        // SET
        {
            //[test_csv_put_basic6
            typedef std::string T; boost::ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            as2::csv_put( assoc, x, z, y );

            BOOST_ASSIGN_V2_CHECK( assoc.count( x ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( z ) == 1 );
            //]
        }
        {
            //[test_csv_put_basic7
            boost::ptr_unordered_set<std::string> set;
            as2::csv_put( set, "foo", "bar", "baz" );

            BOOST_ASSIGN_V2_CHECK( set.count( "foo" ) == 1 );
            BOOST_ASSIGN_V2_CHECK( set.count( "baz" ) == 1 );
            //]
        }

    }// test()

}// xxx_csv_put_basic
}// xxx_put
}// xxx_test_assign
