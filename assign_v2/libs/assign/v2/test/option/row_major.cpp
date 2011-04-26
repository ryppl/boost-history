//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>

#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/option/modifier/row_major.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/multi_array.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/row_major.h>

#include <iostream>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_row_major{

    void test()
    {
        namespace as2 = boost::assign::v2;
        
        //[test_option_row_major
        typedef boost::multi_array<int, 2> array2_;
        typedef array2_::size_type size_;
        typedef size_ const dim_;
        dim_ dim1 = 3, dim2 = 3;
        array2_ array2( boost::extents[dim1][dim2] );

        int k = -2;
        BOOST_AUTO( option, as2::_option % ( as2::_row_major = ( boost::lambda::var( k ) += 2 ) ) );
        as2::csv_put( array2, option, -1, -1, -1, -1, -1 );
        k = -1; as2::csv_put( array2, option, +1, +1, +1, +1 );
        
        const int benchmark [] = {
            -1, +1, -1,
            +1, -1, +1,
            -1, +1, -1
        };
        size_ const n = array2.num_elements();
        BOOST_ASSIGN_V2_CHECK(
            boost::range::equal(
                boost::make_iterator_range( array2.data(), n + array2.data() ),
                boost::make_iterator_range( benchmark, n + benchmark )
            )
        );
        //]
    }

}// xxx_row_major
}// xxx_option
}// xxx_test_assign

