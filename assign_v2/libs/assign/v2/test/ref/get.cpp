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
#include <boost/range/begin.hpp>
#include <boost/assign/v2/ref/array/get.hpp>
#include <boost/assign/v2/ref/wrapper/copy.hpp>
#include <boost/assign/v2/deque/csv_deque.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <libs/assign/v2/test/ref/wrapper.h>

namespace test_assign_v2{
namespace xxx_ref{
namespace xxx_get{

    void test()
    {
    	//[test_ref_get
        namespace as2 = boost::assign::v2;
        typedef int T; typedef as2::ref::copy_wrapper<T>::type w_;
        boost::array<w_, 3> ref_array; int x, y, z;
        
        boost::copy( 
        	as2::csv_deque( w_( x ), w_( y ), w_( z ) ), 
            boost::begin( ref_array ) 
        );
        boost::copy( 
        	as2::csv_deque( 314, 3141, 31415 ), 
            boost::begin( ref_array | as2::ref::_get )
        );

        BOOST_ASSIGN_V2_CHECK( x == 314 );
        BOOST_ASSIGN_V2_CHECK( y == 3141 );
        BOOST_ASSIGN_V2_CHECK( z == 31415 );
        //]
    }

}// xxx_get
}// xxx_ref
}// test_assign_v2