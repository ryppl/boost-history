////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2011 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <string>
#include <utility>
#include <boost/array.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/include/csv_deque_basic.hpp>
#include <boost/assign/v2/include/deque.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/deque/csv_deque_basic.h>

namespace test_assign_v2{
namespace xxx_deque{
namespace xxx_csv_deque_basic{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            //[test_csv_deque_basic1
            typedef as2::result_of::csv_deque<const char[2]>::type C1;
            typedef as2::result_of::deque<char*>::type result1_;
            typedef as2::result_of::csv_deque<std::string>::type C2;
            typedef as2::result_of::deque<std::string>::type result2_;

            BOOST_MPL_ASSERT(( boost::is_same<C1, result1_> ));
            BOOST_MPL_ASSERT(( boost::is_same<C2, result2_> ));

            C1 deque1 = as2::csv_deque( "x", "y", "z" );
            C2 deque2 = as2::csv_deque<std::string>( "x", "y", "z" );

            std::deque<std::string> benchmark;
            benchmark.push_back( "x" );
            benchmark.push_back( "y" );
            benchmark.push_back( "z" );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, deque1 ) );
            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, deque2 ) );
            //]
        }
        {
            //[test_csv_deque_basic2
            typedef std::string T;
            typedef as2::result_of::csv_deque<T>::type C;

            BOOST_MPL_ASSERT((
                boost::is_same<C, as2::result_of::deque<std::string>::type>
            ));

            C  deque = as2::csv_deque<T>( "x", "y", "z" );

            std::deque<std::string> benchmark;
            benchmark.push_back( "x" );
            benchmark.push_back( "y" );
            benchmark.push_back( "z" );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, deque ) );
            //]
        }
        {
            //[test_csv_deque_basic3
            typedef as2::result_of::csv_deque<int>::type C;

            BOOST_MPL_ASSERT((
                boost::is_same<C, as2::result_of::deque<int>::type>
            ));

            C series1 = as2::csv_deque( 0, 1, 1, 2, 3, 5, 8 );

            std::deque<int> benchmark; benchmark.push_back( 0 );
            benchmark.push_back( 1 ); benchmark.push_back( 1 );
            benchmark.push_back( 2 ); benchmark.push_back( 3 );
            benchmark.push_back( 5 ); benchmark.push_back( 8 );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, series1 ) );

            C series2 = as2::csv_deque( 0, 1, 1 )( 2 )( 3 )( 5 )( 8 );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, series2 ) );
            //]
        }
        {
            //[test_csv_deque_converter
            typedef boost::array<int, 5> C;
            C const& ar = /*<<Notice unqualified>>*/converter(
                as2::csv_deque( 1, 2, 3, 4, 5 )
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( ar, as2::csv_deque( 1, 2, 3, 4, 5 ) )
            );
            //]
        }
    }

}// xxx_csv_deque_basic
}// xxx_deque
}// test_assign_v2
