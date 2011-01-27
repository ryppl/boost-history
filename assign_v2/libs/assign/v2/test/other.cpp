//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <utility>
#include <deque>
#include <map>
#include <vector>
#include <string>
#include <boost/typeof/typeof.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/range/size.hpp>
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/container/functor.hpp>
#include <boost/assign/v2/put/ext.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <boost/assign/v2/ref/array/functor/converter.hpp>

#include <libs/assign/v2/test/other.h>

namespace test_assign_v2{
namespace xxx_other{

	// Tests interaction between different functionalities 

    void test(){
    	namespace as2 = boost::assign::v2;
    
    	{	
        	//[array_converter
        	typedef std::pair<int, int> T;
        	typedef std::vector<T> cont_;
        	// notice name lookup
        	cont_ cont = converter( as2::ref::array( T(0, 0) )( T(1, 2) ) );
        	BOOST_ASSIGN_V2_CHECK( cont[1].first == 1 );
        	BOOST_ASSIGN_V2_CHECK( cont[1].second == 2 );
            //]
    	}
        {
        	//[deque_repeat
            BOOST_AUTO(
            	cont,  ( 
                	as2::deque<int>( as2::_nil ) % ( as2::_repeat = 2 ) 
                )( -1 )( 0 )( 1 )
            );
            BOOST_ASSIGN_V2_CHECK( boost::size( cont ) == 6 );
            BOOST_ASSIGN_V2_CHECK( cont.front() == -1 );
            BOOST_ASSIGN_V2_CHECK( cont.back() == 1 );
        }
		{
            //[ref
            int x = -1, y = 0;
            std::deque< boost::tuple<std::string, int&> > cont;
            as2::put( cont )( "x" , x )( "y" , y );
            BOOST_ASSIGN_V2_CHECK( boost::get<0>( cont[0] ) == "x" );
            BOOST_ASSIGN_V2_CHECK( boost::get<0>( cont[1] ) == "y" );
            BOOST_ASSIGN_V2_CHECK( &boost::get<1>( cont[0] ) == &x );
            BOOST_ASSIGN_V2_CHECK( &boost::get<1>( cont[1] ) == &y );
            //]

		}        
	}

}// xxx_other
}// xxx_test_assign
