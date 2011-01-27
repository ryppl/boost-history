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
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>

#include <libs/assign/v2/test/put/deque.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_deque{

	void test()
    {
        namespace as2 = boost::assign::v2;
        {
        	//[csv_deque
            typedef int T; T x = 1, y = 2, z = 0;
            as2::result_of::deque<T>::type cont = as2::csv_deque( x, y, z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
        	//[deque
            typedef int T; T x = 1, y = 2, z = 0;
            as2::result_of::deque<T>::type cont = as2::deque<T>( x )( y )( z );
            BOOST_ASSIGN_V2_CHECK( cont.front() == x );
            BOOST_ASSIGN_V2_CHECK( cont.back() == z );
            //]
        }
        {
        	//[deque_str
        	typedef std::string str_;
            BOOST_ASSIGN_V2_CHECK( 
            	str_( as2::csv_deque( "x" )( "y" )( "z" )[1] ) == "y"
            );
            //]
        }
    }

}// xxx_deque
}// xxx_put
}// test_assign_v2
