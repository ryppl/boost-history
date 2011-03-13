//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <bitset>
#include <cmath>
#include <list>
#include <vector>
#include <boost/array.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container.hpp>
#include <boost/assign/v2/put/fun.hpp>
#include <boost/assign/v2/put/modifier/repeat.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <boost/assign/v2/utility/chain/alias.hpp>
#include <libs/assign/v2/test/mix.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_mix{

    // Tests interaction between different functionalities

    void test(){

        using namespace boost;
        using namespace lambda;
        namespace as2 = assign::v2;

        {
            // suggested by JB:
            //[mix_stable_partition
            std::deque<int> cont;
            range::stable_partition(
                /*<< Calls `cont.push_back( t )` for [^t=0,...,5], and returns `cont` >>*/
                cont | as2::_csv_put( 0, 1, 2, 3, 4, 5 ),
                _1 % 2
            );
            BOOST_ASSIGN_V2_CHECK(range::equal(
                cont,
                /*<< [^1, 3, ..., 4] are held by reference (not copies) >>*/
                as2::ref::csv_array(1, 3, 5, 0, 2, 4)
            ));
            //]
        } 
        {
             //[mix_bitset_revisited
            typedef std::string str_; typedef std::bitset<3> data_; 
            std::vector<data_> consecutive; as2::put( consecutive )( as2::as_arg_list( as2::csv_deque</*Explicit template argument `std::string` is required, because the deduced argument would be `const char*`, and only the former is a valid argument to `data_`'s constructor*/std::string>( "000", "001", "010", "011", "100", "101", "110", "111" ) ) );
        
            for(int i = 0; i < consecutive.size(); i++)
            {
                BOOST_ASSIGN_V2_CHECK( consecutive[i].to_ulong() == i );
            }
            //]
        }
        {
            //[mix_maxtrix3x3
            const int sz = 3;
            typedef array<int, sz>  row_;
            array<row_, sz>  matrix3x3 = converter(
                as2::ref::array
                    ( as2::ref::csv_array( 1, 2, 3 ) | as2::_convert<row_>() )
                    ( as2::ref::csv_array( 4, 5, 6 ) | as2::_convert<row_>() )
                    ( as2::ref::csv_array( 7, 8, 9 ) | as2::_convert<row_>() )
            );
            for(int i = 0; i < 9; i++)
            {
                BOOST_ASSIGN_V2_CHECK( matrix3x3[ i / 3 ][ i % 3 ] == i + 1 );
            }
            //]
        }
        {
            //[mix_ragged
            typedef double data_; typedef std::list<data_> variable_size_; typedef std::vector<variable_size_> ragged_;
            
            ragged_ ragged = converter( 
                as2::deque<variable_size_>
                    ( as2::ref::csv_array( 0.71, 0.63, 0.85 ) | as2::_convert<variable_size_>() ) 
                    ( as2::ref::csv_array( 0.61, 0.69, 0.92, 0.55 ) | as2::_convert<variable_size_>() )
                    ( 1, -99.99 )
                    ( )
            );

            BOOST_ASSIGN_V2_CHECK( ragged[0].size() == 3 );
            BOOST_ASSIGN_V2_CHECK( ragged[1].size() == 4 );
            BOOST_ASSIGN_V2_CHECK( ragged[2].size() == 1 );
            BOOST_ASSIGN_V2_CHECK( ragged[3].size() == 0 );
            //]
            data_ eps = numeric::bounds<data_>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( ragged.front().front() - 0.71 ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged.front().back() - 0.85  ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[2].front() + 99.99     ) < eps ); 
            BOOST_ASSIGN_V2_CHECK( fabs( ragged[2].back() + 99.99      ) < eps ); 
        }
        {
        
            //[mix_fun_iterate
            typedef int T; array<T, 5> powers; powers[0] = 1; powers[1] = 10;
            int i = 2, k = powers[ i - 1 ];   
            /*<<Calls `powers[i] = ( k *= 10 )` for [^ i  = 2, 3, 4 ] >>*/
            ( as2::put( powers ) % ( as2::_fun = var( k ) *= 10 ) % ( as2::_iterate = var( i )++ ) )()()();

            BOOST_ASSIGN_V2_CHECK( powers[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( powers[1] == 10 );
            BOOST_ASSIGN_V2_CHECK( powers[2] == 100 );
            BOOST_ASSIGN_V2_CHECK( powers[3] == 1000 );
            BOOST_ASSIGN_V2_CHECK( powers[4] == 10000 );
            //]
        }
    }

}// xxx_mix
}// xxx_test_assign
