//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
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
#include <boost/assign/v2/deque.hpp>
#include <boost/assign/v2/option/data.hpp>
#include <boost/assign/v2/include/put.hpp>
#include <boost/assign/v2/support/config/check.hpp>

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
            //[test_data1
            std::vector<double> exponent;
            /*<-*/typedef double(*fp)(double);/*->*/
            typedef boost::function<double(double)> f_;

            BOOST_AUTO(
                _data,
                ( as2::_data = f_( /*<-*/fp(/*->*/ log10 /*<-*/)/*->*/ ) )
            );

            csv(
                as2::put( exponent ) % _data
                , 1.0, 10.0, 100.0, 1000.0, 10000.0
            );

            double eps = boost::numeric::bounds<double>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.front() - 0.0 ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.back() - 4.0 ) < eps );
            //]
        }
        {
            //[test_data2
            int k = 1;

            BOOST_AUTO(
                _data,
                (
                    as2::_data = (
                        boost::lambda::var( k ) *= boost::lambda::_1
                    )
                )
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    as2::csv(
                        as2::deque<int>( as2::_nil ) % _data,
                        1, 2, 3, 4, 5
                    ),
                    as2::csv_deque( 1, 2, 6, 24, 120 )
                )
            );
            //]
        }
        {
            //[test_data3
            typedef std::string month_; typedef int days_;
            typedef std::map<month_, days_> map_;
            typedef map_::value_type pair_;

            map_ map;
            as2::csv_put<as2::value_>(
                map,
                pair_( "jan", 31 ),
                pair_( "feb", 28 ),
                pair_( "mar", 31 )
            );

            BOOST_ASSIGN_V2_CHECK( map["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( map["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( map["mar"] == 31 );
            //]
        }
    }

}// xxx_data
}// xxx_option
}// test_assign_v2
