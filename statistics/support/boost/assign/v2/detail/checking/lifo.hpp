//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_LIFO_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_LIFO_ER_2010_HPP
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/traits/container/is_lifo.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 

    template<typename V>
    void do_check(container_tag::lifo,V const& v1)
    {
    	using namespace checking::constants;  
        V v = v1;
    	BOOST_ASSIGN_V2_CHECK( v.top() == h ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == g ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == f ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == e ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == d ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == c ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == b ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.top() == a ); v.pop();
    	BOOST_ASSIGN_V2_CHECK( v.empty() == true );
    }

}// checking
}// v2
}// assign
}// boost    
    
#endif
