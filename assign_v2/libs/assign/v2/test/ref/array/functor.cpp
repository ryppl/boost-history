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
#include <boost/next_prior.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assign/v2/ref/array/array.hpp>
#include <libs/assign/v2/test/ref/array/functor.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_array{
namespace xxx_functor{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            //[ref_array_r
            typedef int const T;
            T w = 11, x = 72, y = 31, z = 48;
            as2::ref::nth_result_of::array<3, T>::type ar3 = as2::ref::array( w )( x )( y );
            {
                T& a = ar3.front(); T& b = ar3.back();
                
                BOOST_ASSIGN_V2_CHECK( &a == &w );
                BOOST_ASSIGN_V2_CHECK( &b == &y );
            }
            as2::ref::nth_result_of::array<4, T>::type ar4 = ar3( z );
            {
                T& a = ar4.front(); T& b = ar4.back();
                
                BOOST_ASSIGN_V2_CHECK( &a == &w );
                BOOST_ASSIGN_V2_CHECK( &b == &z );
            }
            //]
            {
                T& a = ( *boost::begin( ar4 ) );
                T& b = *boost::next( boost::begin( ar4 ), ar4.size() - 1 );
                
                BOOST_ASSIGN_V2_CHECK( &a == &w );
                BOOST_ASSIGN_V2_CHECK( &b == &z );
            }
        }
        {
            //[ref_array_w
            typedef int T; T x, y, z;
            std::vector<T> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;
            boost::copy( r, boost::begin(
                    as2::ref::array( x )( y )( z ) | as2::ref::_get
                )
            );
            
            BOOST_ASSIGN_V2_CHECK( x == r[0] );
            BOOST_ASSIGN_V2_CHECK( z == r[2] );
            //]
        }
        {
            //[ref_array_assign
            typedef int T; T x, y, z;
            as2::ref::array( x )( y )( z ).assign( -1 );
            
            BOOST_ASSIGN_V2_CHECK( x == -1 );
            BOOST_ASSIGN_V2_CHECK( z == -1 );
            //]
        }
    }

}// xxx_functor
}// xxx_array
}// xxx_ref
}// test_assign
