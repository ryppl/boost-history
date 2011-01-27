//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECK 
#include <boost/assert.hpp>
#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_ASSERT( p )

namespace boost{
namespace assign{
namespace v2{
namespace checking{

	struct val_pred{

    	val_pred(){}
		template<typename T, typename U>
    	void operator()(T const& t, U const& u)const
        {
        	BOOST_ASSIGN_V2_CHECK( t == u );
        }
    };

	struct ref_pred{

    	ref_pred(){}
		template<typename T, typename U>
    	void operator()(T const& t, U const& u)const
        {
        	BOOST_ASSIGN_V2_CHECK( &t == &u );
        }
    };

}// checking
}// v2
}// assign
}// boost

#endif

// Override with this in the test suite:
//	#include <boost/test/test_tools.hpp>
//	#define BOOST_ASSIGN_V2_CHECK( p ) BOOST_CHECK( p )

