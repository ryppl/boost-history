//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <map>
#include <deque>
#include <string>

#include <boost/lambda/lambda.hpp>

#include <boost/assign/v2/put/pipe.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/put/modifier/ext/lookup.hpp>
#include <boost/assign/v2/put/modifier/ext/repeat.hpp>
#include <boost/assign/v2/put/modifier/ext/iterate.hpp>
#include <libs/assign/v2/test/put/pipe/ext.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_ext{

    void test(){

    	// This test is also about string literals

    	namespace as2 = boost::assign::v2;
        namespace bl = boost::lambda;
    	{
            //[lookup
            std::map<const char*,int> cont;
            cont["x"] = -1; cont["y"] = 0; cont["z"] = 1;
            cont | (
                as2::_csv_put % ( as2::_lookup = ( bl::_1 +=2 ) )
            )( "z", "x", "y" );
            //]
            BOOST_ASSIGN_V2_CHECK( cont["y"] == 2 );
            BOOST_ASSIGN_V2_CHECK( cont["x"] == 1 );
            BOOST_ASSIGN_V2_CHECK( cont["z"] == 3 );
        }
        typedef std::string str_;
        {
            //[lookup2
            std::deque<const char*> cont;
            cont | as2::_put( "x" )( "y" )( "z" );
            //]
            BOOST_ASSIGN_V2_CHECK( str_( cont[0] ) == "x" );
            BOOST_ASSIGN_V2_CHECK( str_( cont[1] ) == "y" );
            BOOST_ASSIGN_V2_CHECK( str_( cont[2] ) == "z" );
        }
        {
            std::deque<int> cont;
            cont | (
                as2::_csv_put % ( as2::_repeat = 2  )
            )( -1, 0, 1 );
            BOOST_ASSIGN_V2_CHECK( cont[0] == -1 );
            BOOST_ASSIGN_V2_CHECK( cont[1] == -1 );
            BOOST_ASSIGN_V2_CHECK( cont[2] == 0 );
            BOOST_ASSIGN_V2_CHECK( cont[3] == 0 );
            BOOST_ASSIGN_V2_CHECK( cont[4] == 1 );
            BOOST_ASSIGN_V2_CHECK( cont[5] == 1 );
        }
        {
            std::deque<int> cont(3);
            cont[0] = -1;
            cont | (
                as2::_csv_put % ( as2::_iterate = 1  )
            )( 0, 1 );
            BOOST_ASSIGN_V2_CHECK( cont[0] == -1 );
            BOOST_ASSIGN_V2_CHECK( cont[1] == 0 );
            BOOST_ASSIGN_V2_CHECK( cont[2] == 1 );
        }
	}

}// xxx_ext
}// xxx_pipe
}// xxx_put
}// xxx_test_assign
