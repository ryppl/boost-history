////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <string>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/assign/v2/put/deque/deque.hpp>
#include <boost/assign/v2/detail/config/check.hpp>

#include <libs/assign/v2/test/put/deque.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_deque{

    void test()
    {
    	using namespace boost;
        namespace as2 = assign::v2;    
        {
			// http://bioinfo.mbb.yale.edu/~mbg/dom/fun3/area-codes/            
            //[deque_tuple_ref
            typedef const char us_state_ [3]; us_state_ ct = "CT", nj = "NJ", ny = "NY";
            typedef int area_code_; typedef boost::tuple<us_state_/*<<Notice the reference>>*/&,  area_code_> data_; 
            as2::result_of::deque< data_ >::type tri_state_area = /*Calls `tri_state.push_back( data_( s, c ) )` for [^( s, c ) = ( ny, 212 )( ny, 718 )( ny, 516 )( ny, 914 )( nj, 210 )( nj, 908 )( nj, 609 )( ct, 203 ) ]*/as2::deque<data_>( ny, 212 )( ny, 718 )( ny, 516 )( ny, 914 )( nj, 210 )( nj, 908 )( nj, 609 )( ct, 203 );

            using namespace boost;
            BOOST_ASSIGN_V2_CHECK( get<0>( tri_state_area.front() ) == ny );
            BOOST_ASSIGN_V2_CHECK( get<1>( tri_state_area.front() ) == 212 );
            BOOST_ASSIGN_V2_CHECK( get<0>( tri_state_area.back()  ) == ct );
            BOOST_ASSIGN_V2_CHECK( get<1>( tri_state_area.back()  ) == 203 );
            //]
		}
        {
        	//[deque_str
            typedef std::string str_;
            BOOST_ASSIGN_V2_CHECK(
                str_( as2::deque<const char*>( "x" )( "y" )( "z" )[1] ) == "y"
            );
            //]
        }
        {
            //[csv_deque
            typedef as2::result_of::csv_deque<int>::type C;
            {
            	C cont = as2::csv_deque( 72, 31, 48 );

            	BOOST_ASSIGN_V2_CHECK( cont.front() == 72 );
            	BOOST_ASSIGN_V2_CHECK( cont.back() == 48 );
            }
            {
            	C cont = as2::csv_deque( 72, 31 )/* Thanks to `result_of::csv_deque<int>::type == result_of::deque<int>::type`*/( 48 );
            
            	BOOST_ASSIGN_V2_CHECK( cont.front() == 72 );
            	BOOST_ASSIGN_V2_CHECK( cont.back() == 48 );
			}
            //]
        }
        {
            //[csv_deque_str
            typedef std::string str_;
            BOOST_ASSIGN_V2_CHECK(
                str_( as2::csv_deque( "x", "y", "z" )[1] ) == "y"
            );
            //]
        }
    }

}// xxx_deque
}// xxx_put
}// test_assign_v2
