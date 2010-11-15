//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_FIFO_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_FIFO_ER_2010_HPP
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/check_equal.hpp>
#include <boost/assign/v2/detail/type_traits/container/is_fifo.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 

    template<typename V>
    void do_check(container_tag::fifo,V const & v1)
    {
		V v = v1;
    	using namespace constants; 
        BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , a ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , b ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , c ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , d ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , e ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , f ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , g ); v.pop(); 
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.front() , h ); v.pop();
    	BOOST_ASSIGN_V2_CHECK_EQUAL( v.empty() , true); 
	} 

}// checking
}// v2
}// assign
}// boost    
    
#endif
