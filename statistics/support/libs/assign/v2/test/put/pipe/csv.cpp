//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <boost/preprocessor/expr_if.hpp>
#include <boost/mpl/int.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>

#include <libs/assign/v2/test/put/pipe/csv.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_csv{

    void test(){
        using namespace boost;
        using namespace boost::assign::v2;
        using namespace checking::constants;

        typedef boost::mpl::int_<0> zero_;
        using namespace boost::assign::v2;
        int a1, b1, c1, d1;//, e1, f1, g1, h1;
        using namespace checking::constants;
        {
            a1 = a; b1 = b; c1 = c; d1 = d;
        }

#define MACRO1(i) tmp.seq_args()[ i ]
#define MACRO2(i,X) BOOST_ASSIGN_V2_CHECK( &X == &MACRO1(i) );
#define MACRO3(i,X) BOOST_ASSIGN_V2_CHECK( X == MACRO1(i) );

        {
            BOOST_AUTO(
                tmp,
                ( _csv_put( a1, b1, c1 ) )
            );
            MACRO2(0, a1)
            MACRO2(1, b1)
            MACRO2(2, c1)
        }
        {
            BOOST_AUTO(
                tmp,
                ( _csv_put( a1, b1, c ) )
            );
            MACRO2(0, a1)
            MACRO2(1, b1)
            MACRO2(2, c)
        }
#define C -10
        {
            BOOST_AUTO(
                tmp,
                ( _csv_put( a1, b, C ) )
            );
            MACRO2(0, a1)
            MACRO2(1, b)
            MACRO3(2, C)
        }

#undef C
#undef MACRO1
#undef MACRO2
#undef MACRO3

    }

}// xxx_csv
}// xxx_pipe
}// xxx_put
}// xxx_test_assign

