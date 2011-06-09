//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <cmath>
#include <cstddef>
#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/variant.hpp>
#include <libs/assign/v2/tutorial.h>

namespace tutorial_assign_v2{

    void run()
    {
        using namespace boost;
        using namespace assign::v2;
        {
            //[tutorial_csv_put
            std::vector<int> numeric( 10 ); iota( numeric, 0 );
            typedef std::string str_; typedef variant< int, str_ > data_;
            array<data_, 17> keypad;

            /*<<Equivalent to `put( keypad )( "+" )( "-" )...( "." ).for_each( numeric )`>>*/csv(
                put( keypad ),
                "+", "-", "*", "/", "=", ".", "c"
            ).for_each( numeric );

            assert( get<str_>( keypad.front() ) == "+" );
            assert( get<int>( keypad.back() ) == 9 );
            //]
        }
        {
            //[test_do_csv_put_map
            typedef std::map<std::string, int> map_; map_ cal;
            typedef map_::value_type p_;

            boost::for_each(
                /*<<Calls `cal.insert( p_( k, x ) );` for each [^( k, x )] pair in the argurment list, and returns `cal`>>*/cal | do_csv_put<2>( "jan", 31, "feb", 28, "mar", 31 ),
                std::cout
                    << lambda::bind( &p_::first, lambda::_1 ) << ':'
                    << lambda::bind( &p_::second, lambda:: _1 ) << ' '
            ); // prints jan:31 feb:28 mar:31
            //]
        }
        {
            //[tutorial_do_put
            std::deque<int> source( 6 ), target; iota( source, 0 );

            range::stable_partition(
                target | _do_put.for_each( source ),
                lambda::_1 % 2
            );
            //]
            //[tutorial_csv_deque
            assert(
                range::equal( target, csv_deque(1, 3, 5, 0, 2, 4) )
            );
            //]
        }
        {
            //[tutorial_empty_deque
            assert( deque<int>( _nil ).size() == 0 );    // Functor form
            assert( csv_deque<int>().size() == 0 );      // Csv form
            //]
        }
        {
            //[tutorial_ref_csv_array
            int x = 4, y = 6, z = -1;
            int const& max = *max_element( ref::csv_array( x, y, z ) );

            assert( &max == &y );
            //]
            //[tutorial_ref_csv_array_write
            std::vector<int> vec( 3, 0 );
            boost::copy(
                vec,
                boost::begin(
                    ref::csv_array( x, y, z ) | ref::_get
                )
            );

            assert( x == 0 ); assert( y == 0 ); assert( z == 0 );
            //]
        }
        {
            //[tutorial_ptr_container
            typedef std::string word_; ptr_set<word_> words;
            /*<<Calls `assoc.insert( new word_( s ) )` for [^s = "isomer", "ephemeral", "prosaic"]>>*/
            csv_put( words, "isomer", "ephemeral", "prosaic" );

            assert( words.count( "isomer" ) == 1 );
            assert( words.count( "prosaic" ) == 1 );
            //]
        }
        {
            //[tutorial_chain
            std::vector<int> source( 8 ); boost::iota( source, 1 );

            array<int, 4> head; int t, a, i, l;
            boost::copy(
                source,
                boost::begin(
                    head | _chain(
                        ref::csv_array( t, a, i, l ) | ref::_get
                    )
                )
            );

            assert( range::equal( head, csv_deque( 1, 2, 3, 4 ) ) );
            assert( t == 5 ); assert( a == 6 );
            assert( i == 7 ); assert( l == 8 );
            //]
        }
        {
            //[tutorial_converter
            typedef std::queue<int> C;

            C fifo = /*<<Returns a container constructed like this : `C cont; put( cont ).for_each( csv_deque( 1, 2, 3 ) );`>>*/converter( csv_deque( 1, 2, 3 ) );

            assert( fifo.front() == 1 );
            assert( fifo.back() == 3 );
            //]

            //[tutorial_convert
            assert(
                ( csv_deque( 1, 1, 3 ) | convert<C>() ) < fifo
            );
            //]
        }
        {
            //[tutorial_conversion
            const int sz = 3; typedef boost::array<int, sz>  row_;

            convert<row_> as_row;
            boost::array<row_, sz>  matrix3x3 = converter(
                ref::array
                    ( ref::csv_array( 1, 2, 3 ) | as_row )
                    ( ref::csv_array( 4, 5, 6 ) | as_row )
                    ( ref::csv_array( 7, 8, 9 ) | as_row )
            );

            for(int i = 0; i < 9; i++)
            {
                assert( matrix3x3[ i / 3 ][ i % 3 ] == i + 1 );
            }
            //]
        }
        {
            //[tutorial_map
            typedef std::string word_;
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            std::map<int, word_> map;
            put( map )/*<<Calls `map.insert( 1, word_( x, 3 ) )`>>*/( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux" );

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }
        {
            //[tutorial_data
            std::vector<double> exponent;
            /*<-*/typedef double(*fp)(double);//MSVC  /*->*/
            typedef function<double(double)> f_;
            /*<<Equivalent to `exponent.push_back( double( log10( x ) ) )` for [^x = 1.0, ..., 10000.0]>>*/csv(
                put( exponent ) % ( _data = f_( /*<-*/fp(/*->*/log10/*<-*/)/*->*/ ) )
                , 1.0, 10.0, 100.0, 1000.0, 10000.0
            );

            double eps = numeric::bounds<double>::smallest();
            assert( fabs( exponent.front() - 0.0 ) < eps );
            assert( fabs( exponent.back() - 4.0 ) < eps );
            //]
        }
        {
            //[tutorial_push_front
            boost::circular_buffer<int> cb( 3 );

            assert(
                range::equal(
                    cb | do_csv_put<push_front_, 1>( 3, 2, 1 ),
                    csv_deque( 1, 2, 3 )
                )
            );
            //]
        }
        {
            //[tutorial_repeat
            BOOST_AUTO( _modifier, ( _repeat = 2 ) );

            assert(
                boost::range::equal(
                    csv(
                        deque<int, push_front_>( _nil) % _modifier
                        , 1, 10, 100, 1000
                    ),
                    csv_deque( 1000, 1000, 100, 100, 10, 10, 1, 1 )
                )
            );
            //]
        }
    }

}// tutorial_assign_v2
