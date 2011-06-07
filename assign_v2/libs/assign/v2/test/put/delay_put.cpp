//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <map>
#include <string>

#include <boost/assign/v2/include/csv_deque_basic.hpp>
#include <boost/assign/v2/include/csv_deque_ext.hpp>
#include <boost/assign/v2/include/delay_put.hpp>
#include <boost/assign/v2/support/config/check.hpp>
#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/range/algorithm/equal.hpp>

#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <tuple>
#else
#include <boost/tuple/tuple.hpp>
#endif

#include <libs/assign/v2/test/put/delay_put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_delay_put{

    void test(){

        namespace as2 = boost::assign::v2;
        {
            //[test_delay_put1
            boost::circular_buffer<int> cb(3);

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    cb | as2::_delay_put.for_each( as2::csv_deque(1 ,2, 3) )
                    , as2::csv_deque(1, 2, 3)
                )
            );

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    cb | as2::_delay_put.for_each( as2::csv_deque( 4, 5 ) ),
                    as2::csv_deque( 3, 4, 5 )
                )
            );
            //]
        }
        {
            //[test_delay_put2
            typedef std::string month_; typedef int days_;
            typedef std::pair<month_, days_> p_;
//<-
#if BOOST_ASSIGN_V2_ENALBE_CPP0X
//->
            typedef std::tuple<month_, days_> t_;
//<-
#else
            typedef boost::tuple<month_, days_> t_;
#endif
//->
            std::deque<p_> q1;

            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    q1 | as2::_delay_put.for_each<2>(
                        as2::csv_deque<t_, 2>(
                            "jan", 31, "feb", 28, "mar", 31
                        )
                    ),
                    as2::csv_deque(
                        p_("jan", 31), p_("feb", 28), p_("mar", 31 )
                    )
                )
            );
            //]
        }
        {
            //[test_delay_put3
            typedef std::string month_;
            typedef int days_;
            std::map<month_, days_> q1, benchmark;

            benchmark[ "jan" ] = 31;
            benchmark[ "feb" ] = 28;
            benchmark[ "mar" ] = 31;
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
//->
            typedef std::tuple<const char (&)[4], int> t_;
//<-
#else
            typedef boost::tuple<const char (&)[4], int> t_;
#endif
//->
            BOOST_ASSIGN_V2_CHECK(
                boost::range::equal(
                    q1 | as2::_delay_put.for_each<2>(
                        as2::csv_deque<t_, 2>(
                            "jan", 31, "feb", 28, "mar", 31
                        )
                    ),
                    benchmark
                )
            );
            //]
        }
    }// test()

}// xxx_delay_put
}// xxx_put
}// xxx_test_assign
