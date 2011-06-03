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
#include <boost/assign/v2/include/deque.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/deque/deque.h>

namespace test_assign_v2{
namespace xxx_deque{
namespace xxx_deque{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            //[test_deque1
            typedef as2::result_of::deque<char*>::type C;

            C deque = as2::deque<char*>( "x" )( "y" )( "z" );

            std::deque<std::string> benchmark;
            benchmark.push_back( "x" );
            benchmark.push_back( "y" );
            benchmark.push_back( "z" );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, deque ) );
            //]
        }
        {
            //[test_deque2
            typedef std::string word_;
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";

            std::deque<word_> benchmark;
            benchmark.push_back( word_( x, 3 ) );
            benchmark.push_back( word_( y ) );
            benchmark.push_back( word_( z, 3, 3 ) );
            benchmark.push_back( word_( "qux" ) );

            typedef as2::result_of::deque<word_>::type C;
            C cont1 = as2::deque<word_>( as2::_nil );

            BOOST_ASSIGN_V2_CHECK( cont1.empty() );

            cont1( x, 3 )( y )( z, 3, 3 )( "qux" );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, cont1 ) );

            C cont2 = as2::deque<word_>( x, 3 )( y )( z, 3, 3 )( "qux" );

            BOOST_ASSIGN_V2_CHECK( boost::range::equal( benchmark, cont2 ) );
            //]
        }
    }

}// xxx_deque
}// xxx_deque
}// test_assign_v2
