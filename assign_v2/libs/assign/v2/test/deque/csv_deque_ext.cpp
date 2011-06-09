////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2011 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <string>
#include <boost/assign/v2/include/csv_deque_ext.hpp>
#include <boost/assign/v2/support/config/check.hpp>

#include <boost/assign/v2/support/config/enable_cpp0x.hpp>
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
#include <tuple>
#else
#include <boost/tuple/tuple.hpp>
#endif

#include <libs/assign/v2/test/deque/csv_deque_ext.h>

namespace test_assign_v2{
namespace xxx_deque{
namespace xxx_csv_deque_ext{

    void test()
    {
        namespace as2 = boost::assign::v2;
//<-
#if BOOST_ASSIGN_V2_ENABLE_CPP0X
		using std;
#else
		using boost;
#endif 
//->
        
        {
            //[test_csv_deque_ext1
            typedef const char state_ [3]; typedef int code_;
            state_ ct = "CT", nj = "NJ", ny = "NY";

            typedef tuple<state_/*<<Notice the reference>>*/&,  code_> data_;

            as2::result_of::deque<
                data_
            >::type region = as2::csv_deque<data_, 2>(
                ny, 212, ny, 718, ny, 516, ny, 914,
                nj, 210, nj, 908, nj, 609,
                ct, 203
            );

            BOOST_ASSIGN_V2_CHECK( get<0>( region.front() ) == ny );
            BOOST_ASSIGN_V2_CHECK( get<1>( region.back() ) == 203 );
            BOOST_ASSIGN_V2_CHECK( get<1>( region.front() ) == 212 );
            BOOST_ASSIGN_V2_CHECK( get<0>( region.back() ) == ct );
            //]
        }

    }

}// xxx_csv_deque_ext
}// xxx_deque
}// test_assign_v2
