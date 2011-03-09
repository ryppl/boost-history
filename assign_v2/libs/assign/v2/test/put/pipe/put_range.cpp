////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <stack>
#include <vector>
#include <deque>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/pipe/range.hpp>
#include <boost/assign/v2/put/container/put.hpp>
#include <libs/assign/v2/test/put/pipe/put_range.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_put_range{

    void test()
    {
        namespace as2 = boost::assign::v2;


        {
            //[pipe_range_assign
            typedef const char state_ [3]; state_ ct = "CT", nj = "NJ", ny = "NY", ca = "CA", /* ore = "OR",*/ wa = "WA";
            typedef int code_; typedef boost::tuple<state_/*<<Notice the reference>>*/&,  code_> area_code_; 
            std::deque< area_code_ > tri_state; as2::put( tri_state )( nj, 201 )( ct, 203 )( ny, 212 )( ny, 315 )( ny, 347 )( nj, 551 );
            std::deque< area_code_ > pacific ; as2::put( pacific )( wa, 206 )( ca, 209 )( ca, 213 )( wa, 253 );

			std::deque< area_code_ > states; states | as2::_put_range( tri_state ) | as2::_put_range( pacific );

			using namespace boost;
            BOOST_ASSIGN_V2_CHECK( get<0>( states.front()             ) == nj );
            BOOST_ASSIGN_V2_CHECK( get<1>( states.front()             ) == 201 );
            BOOST_ASSIGN_V2_CHECK( get<0>( states[tri_state.size()-1] ) == nj );
            BOOST_ASSIGN_V2_CHECK( get<1>( states[tri_state.size()-1] ) == 551 );
            BOOST_ASSIGN_V2_CHECK( get<0>( states[tri_state.size()]   ) == wa );
            BOOST_ASSIGN_V2_CHECK( get<1>( states[tri_state.size()]   ) == 206 );
            BOOST_ASSIGN_V2_CHECK( get<0>( states.back()              ) == wa );
            BOOST_ASSIGN_V2_CHECK( get<1>( states.back()              ) == 253 );
            //]
        }
        {
            //[pipe_range_constr
            std::vector<int> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;

            BOOST_ASSIGN_V2_CHECK( ( ::boost::type< std::stack<int> >() | as2::_put_range( r ) ).top() == 48 );
            //]
        }
    }

}// xxx_put_range
}// xxx_pipe
}// xxx_put
}// test_assign_v2
