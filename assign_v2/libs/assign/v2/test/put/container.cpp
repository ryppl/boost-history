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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container/put.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/variant.hpp>
#include <libs/assign/v2/test/put/container.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_container{

    void test(){
    
        using namespace boost;
        namespace as2 = assign::v2;

        {
             //[put_bitset
            typedef std::string str_; typedef std::bitset<3> data_; 
            /*<<Keep in mind that although `data_( str_( "011" ) )`, for instance, is valid, `consecutive.push_back( str_( "011" ) )` isn't (GCC4.2)>>*/ std::vector<data_> consecutive; /*<<Calls `consecutive.push_back( data_( t ) );` for [^t = str_( "000" ), ..., str_( "111" )]`>>*/as2::put( consecutive )( str_( "000" ) )( str_( "001" ) )( str_( "010" ) )( str_( "011" ) )( str_( "100" ) )( str_( "101" ) )( str_( "110" ) )( str_( "111" ) );
        
            for(int i = 0; i < consecutive.size(); i++)
            {
                assert( consecutive[i].to_ulong() == i );
            }
            //]
            for(int i = 0; i < consecutive.size(); i++)
            {
                BOOST_ASSIGN_V2_CHECK( consecutive[i].to_ulong() == i );
            }
        }
        {
             //[put_as_arg_list
            std::vector<int> numeric( 10 ); iota( numeric, 0 ); typedef std::string str_;
            typedef variant< int, str_ > data_; array<data_, 16> numeric_kb;
            as2::put( numeric_kb )/*<<Calls [^{`numeric_kb[ i ] = data_( t )` : ( i, t ) = ( 0, "+" ), ..., ( 5, "." )}]>>*/( "+" )( "-" )( "*" )( "/" )( "=" )( "." )/*<<Calls [^{`numeric_kb[ 6 + i ] = data_( *( b + i ) )` : i = 0, ..., 9; b = `boost::begin( numeric )` }]>>*/( as2::as_arg_list( numeric ) );

            assert( get<str_>( numeric_kb.front() ) == "+" );
            assert( get<int>( numeric_kb.back()  ) == 9 );
            //]
            BOOST_ASSIGN_V2_CHECK( get<str_>( numeric_kb.front()  ) == "+" );
            BOOST_ASSIGN_V2_CHECK( get<str_>( numeric_kb[ 5 ] ) == "." );
            BOOST_ASSIGN_V2_CHECK(  get<int>( numeric_kb[ 6 ] ) == 0 );
            BOOST_ASSIGN_V2_CHECK(  get<int>( numeric_kb.back() ) == 9 );
        }
        {
            //[put_variable_size
            typedef double data_; typedef std::vector<data_> variable_size_; 
            variable_size_ a( 3 ); a[0] = 0.71; a[1] = 0.63; a[2] = 0.85;
            variable_size_ b( 4 ); b[0] = 0.61; b[1] = 0.69; b[2] = 0.92; b[3] = 0.55;
            array<variable_size_, 4> ragged; 
            as2::put( ragged )
                /*<<Calls `ragged.push_back( variable_size_( begin( a ), end( a ) ) )`>>*/( begin( a ), end( a ) )
                /*<<Calls `ragged.push_back( variable_size_( b ) )`>>*/( b )
                /*<<Calls `ragged.push_back( variable_size_( 1, -99.99 ) )`>>*/( 1, -99.99 )
                /*<<Calls `ragged.push_back( variable_size_( ) )`>>*/( );

            assert( ragged[0].size() == a.size() );
            assert( ragged[3].size() == 0        );
            //]
            BOOST_ASSIGN_V2_CHECK( ragged[0].size() == a.size() );
            BOOST_ASSIGN_V2_CHECK( ragged[1].size() == b.size() );
            BOOST_ASSIGN_V2_CHECK( ragged[2].size() == 1        );
            BOOST_ASSIGN_V2_CHECK( ragged[3].size() == 0        );
            data_ eps = numeric::bounds<data_>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[0].front() - a.front() ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[0].back()  - a.back()  ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[1].front() - b.front() ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[1].back()  - b.back()  ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[2].front() + 99.99     ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[2].back()  + 99.99     ) < eps );
        }
        {
            //[put_deduce
            std::queue<int> adapter; /*<<Calls `fifo.push( t )` for [^t = 72, 31, 48]>>*/as2::put( adapter )( 72 )( 31 )( 48 );
            array<int, 3> array; /*<<Calls `array[i] = t` for [^( i, t ) = ( 0, 72 ), ( 1, 31 ), ( 2, 48 )]>>*/as2::put( array )( 72 )( 31 )( 48 );
            std::set<int> assoc; /*<<Calls `assoc.insert( t )` for [^t = 72, 31, 48]>>*/as2::put( assoc )( 72 )( 31 )( 48 );
            std::list<int> seq; /*<<Calls `seq.push_back( t )` for [^t = 72, 31, 48]>>*/as2::put( seq )( 72 )( 31 )( 48 );
            //]
            BOOST_ASSIGN_V2_CHECK( adapter.front()   == 72 );
            BOOST_ASSIGN_V2_CHECK( adapter.back()    == 48 );
            BOOST_ASSIGN_V2_CHECK( array.front()     == 72 );
            BOOST_ASSIGN_V2_CHECK( array.back()      == 48 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( 48 ) == 1 );
            BOOST_ASSIGN_V2_CHECK( assoc.count( 72 ) == 1 );
            BOOST_ASSIGN_V2_CHECK( seq.front()       == 72 );
            BOOST_ASSIGN_V2_CHECK( seq.back()        == 48 );
        }
        {
            // http://bioinfo.mbb.yale.edu/~mbg/dom/fun3/area-codes/            
            //[put_seq_tuple_ref
            typedef const char us_state_ [3]; us_state_ ct = "CT", nj = "NJ", ny = "NY";
            typedef int area_code_; typedef tuple<us_state_/*<<Notice the [*reference]>>*/&,  area_code_> data_; std::deque< data_ > tri_state_area; 
            /*<<Calls `tri_state.push_back( data_( s, c ) )` for [^( s, c ) = ( ny, 212 )...( ct, 203 ) ]>>*/
            as2::put( tri_state_area )( ny, 212 )( ny, 718 )( ny, 516 )( ny, 914 )( nj, 210 )( nj, 908 )( nj, 609 )( ct, 203 );

            assert( get<0>( tri_state_area.front() ) == ny );
            assert( get<1>( tri_state_area.back()  ) == 203 );
            //]
            BOOST_ASSIGN_V2_CHECK( get<0>( tri_state_area.front() ) == ny );
            BOOST_ASSIGN_V2_CHECK( get<1>( tri_state_area.front() ) == 212 );
            BOOST_ASSIGN_V2_CHECK( get<0>( tri_state_area.back()  ) == ct );
            BOOST_ASSIGN_V2_CHECK( get<1>( tri_state_area.back()  ) == 203 );
        }
    }// test()

}// xxx_container
}// xxx_put
}// xxx_test_assign
