//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <boost/range/begin.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/ref/array/csv.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <libs/assign/v2/test/ref/array/csv.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_array{
namespace xxx_csv{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            //[ref_csv_array_r
            typedef int const T;
            T x = 1, y = 2, z = 0;
            typedef as2::ref::nth_result_of::csv_array<3, T>::type ar_;
            ar_ ar = as2::ref::csv_array( x, y, z );
            //]
            {    
                //[ref_if_fb
                T& rx = ar.front(); T& rz = ar.back();
                BOOST_ASSIGN_V2_CHECK( &rx == &x );
                BOOST_ASSIGN_V2_CHECK( &rz == &z );
                //]
            }
            {
                //[ref_if_brackets
                T& rx = ar[0]; T& rz = ar[2];
                BOOST_ASSIGN_V2_CHECK( &rx == &x );
                BOOST_ASSIGN_V2_CHECK( &rz == &z );
                //]
            }
            {
                //[ref_if_it
                T& rx = (*boost::begin( ar ) ); 
                T& rz = *boost::next( boost::begin( ar ), 2 );
                BOOST_ASSIGN_V2_CHECK( &rx == &x );
                BOOST_ASSIGN_V2_CHECK( &rz == &z );
                //]
            }
        }
        {
            //[ref_csv_array_w
            typedef int T; T x, y, z;
            std::vector<T> r( 3 ); r[0] = 1; r[1] = 2; r[2] = 0;
            boost::copy( r, boost::begin(
                    as2::ref::csv_array( x, y, z ) | as2::ref::_get 
                )
            );
            BOOST_ASSIGN_V2_CHECK( x == r[0] );
            BOOST_ASSIGN_V2_CHECK( z == r[2] );
            //]
        }
        {
            //[ref_csv_array_as
            typedef int T; T x, y, z;
            as2::ref::csv_array( x, y, z ).assign( -1 );
            BOOST_ASSIGN_V2_CHECK( x == -1 );
            BOOST_ASSIGN_V2_CHECK( z == -1 );
            //]
        }
    }

}// xxx_csv
}// xxx_array
}// xxx_ref
}// test_assign
