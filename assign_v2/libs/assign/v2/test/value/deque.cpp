////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <string>
#include <utility>
#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/value/deque/csv_deque.hpp>
#include <boost/assign/v2/value/deque/deque.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/value/deque.h>

namespace test_assign_v2{
namespace xxx_value{
namespace xxx_deque{

    void test()
    {
    
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_value_deque_pair
            typedef std::string str_;
            typedef std::pair<str_, str_> T;
            typedef as2::result_of::deque<T>::type C;
            C airports = as2::deque<T>("AUH", "Abu Dhabi")("JFK", "New York")("LHR", "Heathrow")( "PEK", "Beijing" );
            
            std::deque<T> benchmark;
            benchmark.push_back( T("AUH", "Abu Dhabi") );
            benchmark.push_back( T("JFK", "New York") );
            benchmark.push_back( T("LHR", "Heathrow") );
            benchmark.push_back( T("PEK", "Beijing") );
            
            range::equal( benchmark, airports );            
            //]
        }        {
            //[test_value_deque_ints
            typedef as2::result_of::deque<int>::type C1;
            typedef as2::result_of::csv_deque<int>::type C2;
            
            BOOST_MPL_ASSERT(( is_same<C1, C2> ));

            C1 empty_cont = as2::deque<int>( as2::_nil );
            C1 cont1 = empty_cont( 72 )( 31 )( 48 );
            C1 cont2 = as2::deque<int>( 72 )( 31 )( 48 );
            C1 cont3 = as2::csv_deque( 72, 31, 48 );
            C1 cont4 = as2::csv_deque( 72, 31 )( 48 );
    
            std::deque<int> benchmark; 
            benchmark.push_back( 72 );
            benchmark.push_back( 31 );
            benchmark.push_back( 48 );

            BOOST_ASSIGN_V2_CHECK( range::equal( benchmark, cont1 ) );
            BOOST_ASSIGN_V2_CHECK( range::equal( benchmark, cont2 ) );
            BOOST_ASSIGN_V2_CHECK( range::equal( benchmark, cont3 ) );
            BOOST_ASSIGN_V2_CHECK( range::equal( benchmark, cont4 ) );
            //]
        }
        {
            //[test_value_deque_str_literal
            typedef as2::result_of::deque<char*>::type C1;
            typedef as2::result_of::csv_deque<const char[2]>::type C2;
            
            BOOST_MPL_ASSERT(( is_same<C1, C2> ));
            
            C1  deque1 = as2::deque<char*>( "x" )( "y" )( "z" );
            C2  deque2 = as2::csv_deque( "x", "y", "z" ); 
            
            std::deque<std::string> benchmark; 
            benchmark.push_back( "x" ); benchmark.push_back( "y" ); benchmark.push_back( "z" );
            
            BOOST_ASSIGN_V2_CHECK( range::equal( benchmark, deque1 ) );
            BOOST_ASSIGN_V2_CHECK( range::equal( benchmark, deque2 ) );
            
            //]
        }
    }

}// xxx_deque
}// xxx_value
}// test_assign_v2
