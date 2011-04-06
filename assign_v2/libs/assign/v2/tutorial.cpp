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
#include <vector>
#include <deque>
#include <map>
#include <queue>
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
            //[tutorial_assign
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
            //[tutorial_piping
            std::deque<int> cont;
            range::stable_partition( cont | _csv_put( 0, 1, 2, 3, 4, 5 ), lambda::_1 % 2 );
            //]
            //[tutorial_container_generation
            assert( range::equal( cont, csv_deque(1, 3, 5, 0, 2, 4) ) );
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
            typedef std::string T; ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            /*<<Calls `assoc.insert( new T( t ) )` for [^t = x, y, z]>>*/
            assoc | _csv_put( x, z, y );

            assert( assoc.count( x ) == 1 );
            assert( assoc.count( z ) == 1 );
            //]
        }
        {
            //[tutorial_chain
            std::vector<int> iota8( 8 ); 
            for(int i = 0; i < 8; i++){ iota8[i] = 1 + i; }
            array<int, 5> iota5; int six, seven, eight;
            boost::copy(
                iota8,
                boost::begin(
                    iota5 | _chain( ref::csv_array( six, seven, eight ) | ref::_get )
                )
            );

            assert( 
                range::equal( iota5, csv_deque(1, 2, 3, 4, 5) ) 
            );
            assert( six == 6 ); 
            assert( seven == 7 ); 
            assert( eight == 8 );
            //]
        }
        {
            //[tutorial_conversion
            std::queue<int> fifo = converter( csv_deque( 1, 10, 100 ) );

            assert( fifo.front() == 1 ); assert( fifo.back() == 100 );
            //]
        }
        {
            //[tutorial_data_gen
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
            //[tutorial_option
            typedef array<int, 10> C; C alternating; 
            alternating[0] = -1; alternating[1] = +1; 
            alternating[2] = -2;

            C::size_type index = 3; 
            ( 
                put( alternating ) % ( _iterate = lambda::var( index )++ ) 
            )( +2 )( -3 )( +3 )( -4 )( +4 )( -5 )( +5 );

            assert( 
                range::equal( 
                    alternating, 
                    csv_deque( -1, +1, -2, +2, -3, +3, -4, +4, -5, +5 ) 
                )
            );
            //]
        }
    }

}// tutorial_assign_v2
