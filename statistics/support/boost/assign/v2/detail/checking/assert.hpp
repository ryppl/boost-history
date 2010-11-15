//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_ASSERT_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_ASSERT_ER_2010_HPP
#include <boost/type_traits/is_same.hpp>
#include <boost/static_assert.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 

	template<typename P>
	void do_assert()
    {
    	BOOST_STATIC_ASSERT( P::value );
    }

    template<typename A,typename B>
    void do_assert_is_same(){
        typedef boost::is_same<A,B> pred_;
        checking::do_assert<pred_>();
	}
    
}// checking
}// v2
}// assign
}// boost    
    
#endif
