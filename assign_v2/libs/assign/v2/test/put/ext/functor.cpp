//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <map>
#include <stack>
#include <boost/lambda/lambda.hpp>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/ext.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <libs/assign/v2/test/put/ext/functor.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_ext{
namespace xxx_functor{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace bl = boost::lambda;
        {
        	//[iterate
        	boost::array<int, 3> cont;
            ( as2::put( cont ) % as2::_iterate )( 1 )( 2 )( 3 );
            BOOST_ASSIGN_V2_CHECK( cont[0] == 1 );
            BOOST_ASSIGN_V2_CHECK( cont[2] == 3 );
            //]
        }
        {	
        	//[lookup
            std::map<std::string, int> cont;
            cont["jan"] = 31; cont["feb"] = 29; cont["mar"] = 32;
            (
                as2::put( cont ) % ( as2::_lookup = ( bl::_1 -= 1 ) ) 
            )( "feb" )( "mar" );
            BOOST_ASSIGN_V2_CHECK( cont["feb"] == 28 );
            BOOST_ASSIGN_V2_CHECK( cont["mar"] == 31 );
            //]
        }
        {
            //[repeat
            std::vector<int> cont;
            ( as2::put( cont ) % ( as2::_repeat = 2  ) )( -1 )( 0 )( 1 );
            BOOST_ASSIGN_V2_CHECK( cont.size() == 6 );
            BOOST_ASSIGN_V2_CHECK( cont.front() == -1 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == 1 );
            //]
        }
    }

}// xxx_functor
}// xxx_ext
}// xxx_put
}// test_assign_v2
