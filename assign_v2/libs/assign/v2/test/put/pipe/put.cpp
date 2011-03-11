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
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp> 
#include <boost/range/algorithm/for_each.hpp>

#include <libs/assign/v2/test/put/pipe/put.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_put{ 

    void test(){
		using namespace boost;
        namespace as2 = assign::v2;
    	{
        	//pipe_put_str_literal
	        typedef const char* T; typedef std::string str_; std::deque<T> cont;
        	BOOST_ASSIGN_V2_CHECK( 
            	boost::range::equal(
            		cont | as2::_put( "x" )( "y" )( "z" ),
                    as2::csv_deque( "x" )( "y" )( "z" )
                )
        	);
        	BOOST_ASSIGN_V2_CHECK( str_( cont[1] ) == "y" );
        	BOOST_ASSIGN_V2_CHECK( str_( cont[2] ) == "z" );
    	}
		{
            //[pipe_put_as_arg_list
			boost::array<int, 2> interval; boost::array<int, 6> all6;
			BOOST_ASSIGN_V2_CHECK(
            	boost::range::equal(
            		all6 | as2::_put( 1 )( 2 )( as2::as_arg_list( interval | as2::_csv_put( 3, 4 ) ) )( 5 )( 6 ),
                    as2::csv_deque( 1, 2, 3, 4, 5, 6 )
                )
            );
			BOOST_ASSIGN_V2_CHECK(
            	boost::range::equal( interval, as2::csv_deque( 3, 4 ) )
            );
			//]
        }

    }// test()

}// xxx_put
}// xxx_pipe
}// xxx_put
}// xxx_test_assign