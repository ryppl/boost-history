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
#include <list>
#include <map>
#include <queue>
#include <vector>
#include <boost/assign/v2.hpp>
#include <boost/array.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/function.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/range/algorithm_ext/iota.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/max_element.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
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
            array<data_, 16> keypad;
            
            /*<<Equivalent to `put( keypad )( "+" )( "-" )...( "." )( 0 )...( 9 )`>>*/csv_put( keypad, "+", "-", "*", "/", "=", "." , as_arg_list( numeric ) );

            assert( get<str_>( keypad.front() ) == "+" );
            assert( get<int>( keypad.back()  ) == 9 );
            //]
        }
        {
            //[tutorial_delay_csv_put
            std::deque<int> source( 6 ), target; iota( source, 0 );
            
            range::stable_partition( 
                target | delay_csv_put( source ), 
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
            //[tutorial_ref_array
            int x = 4, y = 6, z = -1;
            int const& max = *max_element( ref::csv_array( x, y, z ) );

            assert( &max == &y );
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
            assert( t == 5 ); 
            assert( a == 6 ); 
            assert( i == 7 ); 
            assert( l == 8 );
            //]
        }
        {
            //[tutorial_converter
            typedef std::queue<int> C;
            
            C fifo = converter( csv_deque( 1, 2, 3 ) );
            
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
            /*<-*/typedef double(*fp)(double);/*->*/
            typedef function<double(double)> f_;
            /*<<Equivalent to `( put( exponent ) % ( _data = f_( log10 ) ) )( 1.0 ) ...( 10000.0 )`>>*/csv_put(
                exponent
                , _option % ( _data = f_( /*<-*/fp(/*->*/ log10 /*<-*/)/*->*/ ) )
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
                    cb | delay_csv_put( _option % _push_front, csv_deque( 3, 2, 1 ) ), 
                    csv_deque( 1, 2, 3 ) 
                )
            );
            //]
        }    }

}// tutorial_assign_v2
