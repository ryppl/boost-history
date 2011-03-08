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
#include <boost/assign/v2/put/container/range.hpp>
#include <libs/assign/v2/test/put/container/range.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_container{
namespace xxx_range{

    void test()
    {
        namespace as2 = boost::assign::v2;

            //[range_assign
            typedef const char state_ [3]; state_ ct = "CT", nj = "NJ", ny = "NY", ca = "CA", /*ore = "OR",*/ wa = "WA";
            typedef int code_; typedef boost::tuple<state_/*<<Notice the reference>>*/&,  code_> area_code_; 
            std::deque< area_code_ > tri_state; as2::put( tri_state )( nj, 201 )( ct, 203 )( ny, 212 )( ny, 315 )( ny, 347 )( nj, 551 );
            std::deque< area_code_ > pacific ; as2::put( pacific )( wa, 206 )( ca, 209 )( ca, 213 )( wa, 253 );

			std::deque< area_code_ > states;  as2::put_range( tri_state, states ); as2::put_range( pacific, states );

			using namespace boost;
            BOOST_ASSIGN_V2_CHECK( get<0>( states.front()                                 )  == nj );
            BOOST_ASSIGN_V2_CHECK( get<0>( states[tri_state.size()-1]                     )  == nj );
            BOOST_ASSIGN_V2_CHECK( get<0>( states.front()                                 )  == nj );
            BOOST_ASSIGN_V2_CHECK( get<0>( states[tri_state.size()]                       )  == wa );
            BOOST_ASSIGN_V2_CHECK( get<0>( states[tri_state.size() + pacific.size() - 1 ] ) == wa );
			//]
        {
            //[range_constr
            std::vector<int> r( 3 ); r[0] = 72; r[1] = 31; r[2] = 48;

            BOOST_ASSIGN_V2_CHECK( as2::put_range< std:: stack<int> >( r ).top() == 48 );
            //]
        }
    }

}// xxx_range
}// xxx_container
}// xxx_put
}// test_assign_v2
