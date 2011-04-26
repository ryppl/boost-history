//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cmath> // MSVC #include <math.h>
#include <deque>
#include <list>
#include <map>
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/include/csv_put.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/option/data.h>

namespace test_assign_v2{
namespace xxx_option{
namespace xxx_data{

    void test()
    {
        namespace as2 = boost::assign::v2;
        {
            // (*fp) resolves error C2440 using MSVC
            //[test_option_data_math
            std::vector<double> exponent;
            /*<-*/typedef double(*fp)(double);/*->*/
            typedef boost::function<double(double)> f_;
            as2::csv_put(
                exponent
                , as2::_option % ( as2::_data = f_( /*<-*/fp(/*->*/ log10 /*<-*/)/*->*/ ) )
                , 1.0, 10.0, 100.0, 1000.0, 10000.0
            );

            double eps = boost::numeric::bounds<double>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.front() - 0.0 ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.back() - 4.0 ) < eps );
            //]
        }
        {
            //[test_option_data_recursive
            int k = 1;

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    as2::csv_deque(
                        as2::_option % ( as2::_data = ( boost::lambda::var( k ) *= boost::lambda::_1 ) ),
                        1, 2, 3, 4, 5
                    ),
                    as2::csv_deque( 1, 2, 6, 24, 120 )
                )
            );
            //]
        }
        {
            //[test_option_data_value
            typedef std::string word_;
            const char x[] = "foo";
            const char y[4] = { 'b', 'a', 'r', '\0' };
            word_ z = "***baz";
            typedef std::map<int, word_> C;
            typedef C::value_type T;
            typedef C::mapped_type D;
            C map;
            (
                as2::put( map )  % ( as2::_data = as2::_value )
            )( 1, D( x, 3 ) )( 2, y )( 3, D( z, 3, 3 ) )( 4, "qux");

            BOOST_ASSIGN_V2_CHECK( map[1] == "foo" ); BOOST_ASSIGN_V2_CHECK( map[2] == "bar" );
            BOOST_ASSIGN_V2_CHECK( map[3] == "baz" ); BOOST_ASSIGN_V2_CHECK( map[4] == "qux" );
            //]
        }

    }

}// xxx_data
}// xxx_option
}// test_assign_v2
