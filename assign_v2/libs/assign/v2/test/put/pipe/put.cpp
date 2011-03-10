//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
//#include <map>
#include <deque>
#include <vector>
#include <list>
#include <string>
#include <boost/array.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp> 
#include <boost/range/algorithm/for_each.hpp>

#include <libs/assign/v2/test/put/pipe/put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_put{ 

    void test(){
        namespace as2 = boost::assign::v2;
    	{
        	//pipe_str_literal
	        typedef const char* T;
			std::deque<T> cont; typedef std::string str_;
        	BOOST_ASSIGN_V2_CHECK( str_( 
            	( cont | as2::_put( "x" )( "y" )( "z" ) )[0] 
        	) == "x" );
        	BOOST_ASSIGN_V2_CHECK( str_( cont[1] ) == "y" );
        	BOOST_ASSIGN_V2_CHECK( str_( cont[2] ) == "z" );
    	}
/*
		{
        	// TODO put in mix or chain
            //[pipe_array
			boost::array<int, 3> first3;
            boost::array<int, 5> all5;
            int i = first3.size() - 1;
            ( 	
            	put( all5 )( 
            		as_arg_list( first3 | _csv_put( v, w ) ) 
            	) % _iterate = var(i)++
            )( x, y, z );

            put( all5 )( 
                as_arg_list( first3 | _csv_put( v, w ) && csv_array( x, y, z ) )
			);
			//]
        }
*/
		{
            using namespace boost;
            //[pipe_seq_ref_tuple
            typedef const char state_ [3]; state_ ct = "CT", nj = "NJ", ny = "NY";
            typedef int code_; typedef boost::tuple<state_/*<<Notice the reference>>*/&,  code_> area_code_; 
            std::deque< area_code_ > tri_state;
            area_code_ front = (
            	tri_state | as2::_put( nj, 201 )( ct, 203 )( ny, 212 )( ny, 315 )( ny, 347 )( nj, 551 )
            ).front();

            BOOST_ASSIGN_V2_CHECK( get<0>( front ) == nj );
            BOOST_ASSIGN_V2_CHECK( get<1>( front ) == 201 );
            BOOST_ASSIGN_V2_CHECK( get<0>( tri_state.back() ) == nj );
            BOOST_ASSIGN_V2_CHECK( get<1>( tri_state.back() ) == 551 );
            //]
        }

    }// test()

}// xxx_put
}// xxx_pipe
}// xxx_put
}// xxx_test_assign