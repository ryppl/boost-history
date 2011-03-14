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
#include <boost/assign/v2/ref/array/csv_array.hpp>
#include <boost/assign/v2/ref/wrapper.hpp>
#include <libs/assign/v2/test/ref/csv_array.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_csv_array{

    void test()
    {
        using namespace boost;
        namespace as2 = assign::v2;
        {
            //[test_ref_csv_array_read
            typedef int const T;
            T x = 72, y = 31, z = 48;
            typedef as2::ref::nth_result_of::csv_array<3, T>::type ar_;
            ar_ ar = as2::ref::csv_array( x, y, z );
            /*<-*/{/*->*/
                T& a = ar[0]; T& b = ar[2];

                BOOST_ASSIGN_V2_CHECK( &a == &x );
                BOOST_ASSIGN_V2_CHECK( &b == &z );
            /*<-*/}/*->*/
            //]
            {
                T& a = ar.front(); T& b = ar.back();

                BOOST_ASSIGN_V2_CHECK( &a == &x );
                BOOST_ASSIGN_V2_CHECK( &b == &z );
            }
            {
                T& a = (*begin( ar ) );
                T& b = *next( begin( ar ), 2 );

                BOOST_ASSIGN_V2_CHECK( &a == &x );
                BOOST_ASSIGN_V2_CHECK( &b == &z );
            }
        }
        {
            //[test_ref_csv_array_write
            typedef int T; T x, y, z;
            std::vector<T> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;
            boost::copy( r, begin( as2::ref::csv_array( x, y, z ) | as2::ref::_get ) );

            BOOST_ASSIGN_V2_CHECK( x == r[0] );
            BOOST_ASSIGN_V2_CHECK( z == r[2] );
            //]
        }
        {
            //[test_ref_csv_array_assign
            int x, y, z; as2::ref::csv_array( x, y, z ).assign( -1 );

            BOOST_ASSIGN_V2_CHECK( x == -1 );
            BOOST_ASSIGN_V2_CHECK( z == -1 );
            //]
        }
    }

}// xxx_csv_array
}// xxx_ref
}// test_assign
