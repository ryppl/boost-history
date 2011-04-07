//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <cstddef>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <vector>
#include <boost/assign/v2.hpp>
#include <boost/array.hpp>
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

    // MSVC REMINDER : fully qualify boost::begin boost::end - error C2668

    void run()
    {
        using namespace boost;
        using namespace assign::v2;
        {
            //[tutorial_assign1
            std::vector<int> numeric( 10 ); iota( numeric, 0 ); 
            typedef std::string str_; typedef variant< int, str_ > data_; 
            array<data_, 16> keypad;
            
            csv_put( keypad
                , "+", "-", "*", "/", "=", "." 
                , as_arg_list( numeric ) 
            );

            assert( get<str_>( keypad.front() ) == "+" );
            assert( get<int>( keypad.back()  ) == 9 );
            //]
        }
        {
            //[tutorial_delay_csv_put
            std::deque<int> source( 6 ), sorted; iota( source, 0 );
            
            range::stable_partition( 
            	sorted | delay_csv_put( source ), 
                lambda::_1 % 2 
            );
			//]
			//[tutorial_csv_deque
            assert( 
            	range::equal( sorted, csv_deque(1, 3, 5, 0, 2, 4) ) 
            );
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
            
            array<int, 5> copies; int x, y, z;
            boost::copy(
                source,
                boost::begin(
                    copies | _chain( 
                    	ref::csv_array( x, y, z ) | ref::_get 
                    )
                )
            );

            assert( range::equal( copies, csv_deque(1, 2, 3, 4, 5) ) );
            assert( x == 6 ); assert( y == 7 ); assert( z == 8 );
            //]
        }
        {
            //[tutorial_converter
            typedef std::queue<int> C;
            
            C fifo = converter( csv_deque( 1, 2, 3 ) );
            assert( fifo.front() == 1 ); assert( fifo.back() == 3 );
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
            put( map )/*<<Calls `map.insert( 1, str_( foo, 3 ) )`>>*/( 1, x, 3 )( 2, y )( 3, z, 3, 3 )( 4, "qux");

            assert( map[1] == "foo" ); assert( map[2] == "bar" );
            assert( map[3] == "baz" ); assert( map[4] == "qux" );
            //]
        }
        {
            //[tutorial_modifier
            typedef array<int, 10> C; C series; 
            series[0] = -1; series[1] = +1; 
            series[2] = -2;

            C::size_type index = 3; 
            csv_put( 
            	series
                , _iterate = lambda::var( index )++
            	, +2, -3, +3, -4, +4, -5 , +5 
            );

            assert( 
                range::equal( 
                    series, 
                    csv_deque( -1, +1, -2, +2, -3, +3, -4, +4, -5, +5 ) 
                )
            );
            //]
        }
    }

}// tutorial_assign_v2
