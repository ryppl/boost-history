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
#include <cmath>
#include <boost/lambda/lambda.hpp>
#include <boost/tuple/tuple.hpp>

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
            typedef std::map<std::string, int> C; C cal; 
            C::mapped_type n_mar = (
            	cal | as2::_put( "feb", 28 )
                	| ( /*Input is by default mapped to C::value_type but, here, we need C::key_type*/ as2::_put % /*Makes the input convertible to C::key_type*/as2::_identity %  ( as2::_lookup = (lambda::_1 = 31) ) )( "jan" )( "mar" )( "may" )( "jul" )( "aug" )( "oct" )( "dec" )
                	| ( as2::_put % as2::_identity %  ( as2::_lookup = (lambda::_1 = 30) ) )( "apr" )( "jun" )( "sep" )( "nov" )
            )["mar"];
            
            BOOST_ASSIGN_V2_CHECK( n_mar == 31 );
            BOOST_ASSIGN_V2_CHECK( cal["jan"] == 31 );
            BOOST_ASSIGN_V2_CHECK( cal["dec"] == 31 );
            BOOST_ASSIGN_V2_CHECK( cal["apr"] == 30 );
            BOOST_ASSIGN_V2_CHECK( cal["nov"] == 30 );
			//] 
        }    
    }

}// xxx_lookup
}// xxx_modifier
}// xxx_pipe
}// xxx_put
}// test_assign_v2
