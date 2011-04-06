//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <bitset>
#include <cmath>
#include <deque>
#include <map>
#include <list>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <boost/circular_buffer.hpp>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/traits.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
#include <boost/assign/v2/put/put.hpp>
#include <boost/assign/v2/put/csv_put.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/variant.hpp>
#include <libs/assign/v2/test/put/put.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_put{

    // MSVC REMINDER : fully qualify boost::begin boost::end - error C2668

    void test(){

        using namespace boost;
        namespace as2 = assign::v2;

		// STL
        {
            //[test_csv_put_stl
            std::queue<int> adapter; 	as2::csv_put( adapter, 1, 10, 100 );
            array<int, 3> array; 		as2::csv_put( array,   1, 10, 100 );
            std::set<int> assoc; 		as2::csv_put( assoc,   1, 10, 100 );
            std::list<int> seq; 		as2::csv_put( seq,     1, 10, 100 );
            //]
            BOOST_ASSIGN_V2_CHECK(  adapter.front()    == 1 );
            BOOST_ASSIGN_V2_CHECK(  adapter.back()     == 100 );
            BOOST_ASSIGN_V2_CHECK(  array.front()      == 1 );
            BOOST_ASSIGN_V2_CHECK(  array.back()       == 100 );
            BOOST_ASSIGN_V2_CHECK(  assoc.count( 100 ) == 1 );
            BOOST_ASSIGN_V2_CHECK(  assoc.count( 1 )   == 1 );
            BOOST_ASSIGN_V2_CHECK(  seq.front()        == 1 );
            BOOST_ASSIGN_V2_CHECK(  seq.back()         == 100 );
        }
		// ARRAY
        {
             //[test_csv_put_keypad
            std::vector<int> numeric( 10 ); iota( numeric, 0 ); typedef std::string str_;
            typedef variant< int, str_ > key_; array<key_, 16> keypad;
            as2::csv_put( keypad
                , "+", "-", "*", "/", "=", "." 
                , as2::as_arg_list( numeric ) 
            );

            BOOST_ASSIGN_V2_CHECK(  get<str_>( keypad.front() ) == "+" );
            BOOST_ASSIGN_V2_CHECK(  get<int>( keypad.back() ) == 9 );
            //]
            BOOST_ASSIGN_V2_CHECK(  get<str_>( keypad[ 5 ] ) == "." );
            BOOST_ASSIGN_V2_CHECK(  get<int>( keypad[ 6 ] ) == 0 );
        }
        {
            //[test_put_ragged
            typedef double data_; typedef std::vector<data_> uneven_;
            uneven_ a( 3 ); a[0] = 0.71; a[1] = 0.63; a[2] = 0.85;
            uneven_ b( 4 ); b[0] = 0.61; b[1] = 0.69; b[2] = 0.92; b[3] = 0.55;
            array<uneven_, 4> ragged;
            as2::put( ragged )
                /*<<Calls `ragged.push_back( uneven_( begin( a ), end( a ) ) )`>>*/( boost::begin( a ), boost::end( a ) )
                ( b )
                ( 1, -99.99 )
                ( );

            BOOST_ASSIGN_V2_CHECK( ragged[0].size() == a.size() );
            BOOST_ASSIGN_V2_CHECK( ragged[3].size() == 0 );
            //]
            BOOST_ASSIGN_V2_CHECK(  ragged[1].size() == b.size() );
            BOOST_ASSIGN_V2_CHECK(  ragged[2].size() == 1        );
            data_ eps = numeric::bounds<data_>::smallest();
            BOOST_ASSIGN_V2_CHECK(  fabs( ragged[0].front() - a.front() ) < eps );
            BOOST_ASSIGN_V2_CHECK(  fabs( ragged[0].back()  - a.back()  ) < eps );
            BOOST_ASSIGN_V2_CHECK(  fabs( ragged[1].front() - b.front() ) < eps );
            BOOST_ASSIGN_V2_CHECK(  fabs( ragged[1].back()  - b.back()  ) < eps );
            BOOST_ASSIGN_V2_CHECK(  fabs( ragged[2].front() + 99.99     ) < eps );
            BOOST_ASSIGN_V2_CHECK(  fabs( ragged[2].back()  + 99.99     ) < eps );
        }
		// SEQUENCE
        {
            //[test_csv_put_bitset
            typedef std::string str_; typedef std::bitset<3> number; 
            std::vector<number> /*<<Note that although `number( str_( "011" ) )`, for instance, is valid, `range_3bit.push_back( str_( "011" ) )`>>*/ range_3bit;
            /*<<Calls `range_3bit.push_back( number( t ) );` for [^t = ]`str_( "000" )`[^, ..., ]`str_( "111" )`>>*/as2::csv_put( range_3bit
                , str_( "000" ), str_( "001" )
                , str_( "010" ), str_( "011" )
                , str_( "100" ), str_( "101" )
                , str_( "110" ), str_( "111" )
            );

            for(unsigned i = 0; i < range_3bit.size(); i++)
            {
                BOOST_ASSIGN_V2_CHECK(  range_3bit[i].to_ulong() == i );
            }
            //]
            // unsigned i else warning comparison between signed & unsigned
        }
        {
            // http://bioinfo.mbb.yale.edu/~mbg/dom/fun3/area-codes/
            //[test_put_area_codes
            typedef const char us_state_ [3]; us_state_ ct = "CT", nj = "NJ", ny = "NY";
            typedef int area_code_; 
            typedef tuple<us_state_/*<<Notice the [*reference]>>*/&,  area_code_> data_; 
            std::deque< data_ > region;
            /*<<Calls `tri_state.push_back( data_( s, c ) )` for [^( s, c ) =( ny, 212 )...( ct, 203 )]>>*/
            as2::put( region )
                ( ny, 212 )( ny, 718 )( ny, 516 )( ny, 914 )
                ( nj, 210 )( nj, 908 )( nj, 609 )
                ( ct, 203 );

            BOOST_ASSIGN_V2_CHECK(  get<0>( region.front() ) == ny );
            BOOST_ASSIGN_V2_CHECK(  get<1>( region.back()  ) == 203 );
            //]
            BOOST_ASSIGN_V2_CHECK(  get<1>( region.front() ) == 212 );
            BOOST_ASSIGN_V2_CHECK(  get<0>( region.back()  ) == ct );
        }
        //SET
        {    
            //[test_csv_put_unordered_set
            boost::unordered_set<std::string> set; 
            as2::csv_put( set, "foo", "bar", "baz" );

            BOOST_ASSIGN_V2_CHECK( set.count( "foo" ) == 1 );
            BOOST_ASSIGN_V2_CHECK( set.count( "baz" ) == 1 );
            //]
        }
		// MAP
        {    
            //[test_put_unordered_map
            typedef std::string word_; 
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            boost::unordered_map<int, word_> map;
            as2::put( map )/*<<Calls `map.insert( 1, str_( foo, 3 ) )`>>*/( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux");

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }
        {
            //[test_csv_put_map
            typedef std::string month_; typedef int days_;
            typedef std::map<month_, days_> C; 
            typedef C::value_type T;
            C map; as2::csv_put( map, T("jan", 31 ), T( "feb", 28 ), T( "mar", 31 ) );

            BOOST_ASSIGN_V2_CHECK( map["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( map["mar"] == 31 );
            //]
        }
        // OTHER
        {
            //test_csv_put_cb
            boost::circular_buffer<int> cb(3);
            as2::csv_put( cb, 1, 2, 3 );
             
            BOOST_ASSIGN_V2_CHECK(
            	range::equal(cb, as2::csv_deque(1, 2, 3) )
            );

            as2::csv_put( cb, 4, 5 );

            BOOST_ASSIGN_V2_CHECK(
            	range::equal(cb, as2::csv_deque(3, 4, 5) )
            );
            //]
        }
    }// test()

}// xxx_put
}// xxx_value
}// xxx_test_assign
