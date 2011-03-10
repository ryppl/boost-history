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
#include <string>
#include <boost/lambda/lambda.hpp>

#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/functor/identity.hpp>
#include <boost/assign/v2/put/fun/identity.hpp>
#include <boost/assign/v2/put/modifier/lookup.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <libs/assign/v2/test/put/pipe/modifier/lookup.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modifier{
namespace xxx_lookup{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
            //[lookup
            typedef std::string str_; std::map<std::string, int> C; C cal; 
            BOOST_AUTO( _local,  ( as2::_csv_put % ( as2::_fun = lambda::_1 ) ) );
            BOOST_ASSIGN_V2_CHECK(
            	(
            		cal | as2::_put( "feb", 28 ) | ( _local % as2::_lookup = (lambda::_1 = 30)  )( "apr" )( "jun" )( "sep" )( "nov" )
                    	| ( _local % as2::_lookup = (lambda::_1 = 31)  )( "jan" )( "mar" )( "may" )( "jul" )( "aug" )( "oct" )( "dec" )
            	)[ "mar" ] == 31
            );
            BOOST_ASSIGN_V2_CHECK( cal[ "jun" ] == 30 );
			//] 
        }    
    }

}// xxx_lookup
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2
