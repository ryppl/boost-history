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
            std::vector<int> numeric( 10 ); iota( numeric, 0 ); typedef std::string str_;
            typedef variant< int, str_ > data_; array<data_, 16> numeric_kb;
            put( numeric_kb )
                ( "+" )( "-" )( "*" )( "/" )( "=" )( "." )
                ( as_arg_list( numeric ) );

            assert( get<str_>( numeric_kb.front() ) == "+" );
            assert( get<int>( numeric_kb.back()  ) == 9 );
            //]
        }
        {
            //[tutorial_ptr_container
            typedef std::string T; ptr_set<T> assoc;
            T x = "isomer", y = "ephemeral", z = "prosaic";
            csv( put( assoc ), x, z, y );
            
            assert( assoc.count( x ) == 1 );
            assert( assoc.count( z ) == 1 );
            //]
        }
        {
            //[tutorial_piping
            using namespace lambda;
            std::deque<int> cont; 
            range::stable_partition( cont | _csv_put( 0, 1, 2, 3, 4, 5 ), _1 % 2 );
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
            //[tutorial_chain
            std::vector<int> consecutive8( 8 ); for(int i = 0; i < 8; i++){ consecutive8[i] = 1 + i; }
            array<int, 5> consecutive5; int six, seven, eight;
            boost::copy(
                consecutive8,
               boost::begin( consecutive5 | _chain( ref::csv_array( six, seven, eight ) | ref::_get ) )
            );

            assert( range::equal( consecutive5, csv_deque(1, 2, 3, 4, 5) ) );
            assert( six == 6 ); assert( seven == 7 ); assert( eight == 8 );
            //]
        }
        {
            //[tutorial_conversion
            std::queue<int> fifo = converter( csv_deque( 72, 31, 48 ) );
            
            assert( fifo.front() == 72 ); assert( fifo.back() == 48 );
            //]
        }
        {
            //[tutorial_data_gen
            std::map<std::string, int> map; 
            put( map )( "foo", 1 )( "bar", 2 )( "baz", 3 );
            
            assert( map["bar"] = 2 );
            //]
        }
        {
            //[tutorial_option
            using namespace lambda;
            array<int, 4> powers = converter( csv_deque( 1, 10, -1, -1 ) );
            std::size_t index = 2; using namespace lambda;
             ( put( powers ) % ( _iterate = var( index )++ ) )( 100 )( 1000 );
 
            for(int value = 1, index = 0; index < powers.size(); index++, value *=10 )
            { 
                assert( powers[index] == value ); 
            }
            //]
        }
    }

}// tutorial_assign_v2
