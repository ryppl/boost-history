//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iterator>
#include <vector>
#include <list>
#include <string>
#include <boost/assign/v2/include/ref/csv_array.hpp>
#include <boost/assign/v2/include/csv_deque.hpp>
#include <boost/assign/v2/chain/check.hpp>
#include <boost/assign/v2/chain.hpp>
#include <boost/assign/v2/chain/logical_and.hpp>
#include <boost/next_prior.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <libs/assign/v2/test/chain.h>

namespace test_assign_v2{
namespace xxx_chain{

    // MSVC REMINDER : fully qualify boost::begin boost::end - error C2668

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            namespace ns = as2::check_chain_aux;
            {    typedef int T; ns::static_<T>(); ns::static_<T>(); }
            {    typedef double T; ns::static_<T>(); ns::static_<T>(); }
        }
        // Non-Boost.Assign.v2 containers
        {
            //[test_chain_read
            typedef std::string T;
            std::vector<T> word; 

            boost::copy( 
                as2::csv_deque( "O", "R" ) | as2::_chain( as2::csv_deque( "B", "I", "T" ) ), 
                std::back_inserter( word ) 
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal( word, as2::csv_deque<T>( "O", "R", "B", "I", "T" ) )
            );
            //]
        }
        {
            //[test_chain_write
            typedef int T; boost::array<T, 3> head; std::list<T> tail( 2 );
            
            boost::copy( 
                as2::csv_deque( 1, 2, 3, 4, 5 ),
                boost::begin( head | as2::_chain( tail ) ) 
            );

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal( head, as2::csv_deque( 1, 2, 3 ) ) 
            );
            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal( tail, as2::csv_deque( 4, 5 ) ) 
            );
            //]
        }
        // Boost.Assign.v2 containers
        {    
            //[test_chain_write_refs
            std::vector<int> source( 8 ); boost::iota(source, 1);
            boost::array<int, 4> head; int t, a, i, l;
            
            /*<<Brings `&&` to scope>>*/using namespace boost::assign::v2;
            boost::copy(
                source,
                boost::begin( 
                    head && (/*<< rvalue! >>*/ as2::ref::csv_array( t, a, i, l ) | as2::ref::_get ) 
                )
            );

            BOOST_ASSIGN_V2_CHECK( 
                boost::range::equal( head, as2::csv_deque( 1, 2, 3, 4 ) ) 
            );
            BOOST_ASSIGN_V2_CHECK( t == 5 ); 
            BOOST_ASSIGN_V2_CHECK( a == 6 ); 
            BOOST_ASSIGN_V2_CHECK( i == 7 ); 
            BOOST_ASSIGN_V2_CHECK( l == 8 );
            //]
        }


    }// test

}// xxx_chain
}// test_assign_v2
