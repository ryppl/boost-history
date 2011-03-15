//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iterator>
#include <vector>
#include <list>
#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/assign/v2/ref/array/csv_array.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/assign/v2/utility/chain/check.hpp>
#include <boost/assign/v2/utility/chain.hpp>
#include <boost/assign/v2/utility/chain/operator_and.hpp>
#include <libs/assign/v2/test/utility/chain.h>

namespace test_assign_v2{
namespace xxx_utility{
namespace xxx_chain{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            namespace ns = as2::check_chain_aux;
            {    typedef int T; ns::static_<T>(); ns::static_<T>(); }
            {    typedef double T; ns::static_<T>(); ns::static_<T>(); }
        }
        // Non-Boost.Assign.v2 containers
        {
            //[test_utility_chain_read
            typedef std::string T;
            array<T, 2> head;  head[0] = "A"; head[1] = "B";
            std::list<T> tail; tail.push_back( "C" ); tail.push_back( "D" );
            std::vector<T> joined; copy( head | as2::_chain( tail ), std::back_inserter( joined ) );

            BOOST_ASSIGN_V2_CHECK(
                range::equal( joined, as2::csv_deque<T>("A", "B", "C", "D") )
            );
            //]
        }
        {
            //[test_utility_chain_write
            typedef std::string word; std::vector<word> words( 6 );
            words[0] = "foo"; words[1] = "bar"; words[2] = "baz";
            words[3] = "qux"; words[4] = "quux"; words[5] = "grault";
            array<word, 3> head; std::list<word> tail( 3 );
            copy( words, begin( head | as2::_chain( tail ) ) );

            BOOST_ASSIGN_V2_CHECK( head.front() == "foo" );
            BOOST_ASSIGN_V2_CHECK( head.back() == "baz" );
            BOOST_ASSIGN_V2_CHECK( tail.front() == "qux" );
            BOOST_ASSIGN_V2_CHECK( tail.back() == "grault" );
            //]
        }
        // Boost.Assign.v2 containers
        {
            //[test_utility_chain_write_refs
            /*<< Needed to bring && into scope >>*/ using namespace assign::v2;
            std::vector<int> consecutive8( 8 ); for(int i = 0; i < 8; i++){ consecutive8[i] = 1 + i; }
            array<int, 5> consecutive5; int six, seven, eight;
            boost::copy(
                consecutive8,
                begin( consecutive5 && (/*<< rvalue! >>*/ as2::ref::csv_array( six, seven, eight ) | as2::ref::_get ) )
            );

            BOOST_ASSIGN_V2_CHECK( consecutive5.front() == 1 );
            BOOST_ASSIGN_V2_CHECK( consecutive5.back()  == 5 );
            BOOST_ASSIGN_V2_CHECK( six                  == 6 );
            BOOST_ASSIGN_V2_CHECK( eight                == 8 );
            //]
        }


    }// test

}// xxx_chain
}// xxx_utility
}// test_assign_v2
