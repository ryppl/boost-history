//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_CONTAINER_ER_2010_HPP
#include <boost/assign/v2/detail/checking/deduce_check.hpp>
#include <boost/assign/v2/detail/checking/array.hpp>
#include <boost/assign/v2/detail/checking/fifo.hpp>
#include <boost/assign/v2/detail/checking/lifo.hpp>
#include <boost/assign/v2/detail/checking/map.hpp>
#include <boost/assign/v2/detail/checking/range.hpp>

#include <boost/assign/v2/detail/checking/fwd.hpp> // consistencey check

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 
namespace container{

    template<typename V>
    void do_check(V const & v)
    {  
        typedef typename v2::checking::deduce_check<V>::type tag;
        v2::checking::do_check( tag(), v );
	} 

    template<typename V>
    void do_check(V & v) // This is needed for operator[]
    {  
        typedef typename v2::checking::deduce_check<V>::type tag;
        v2::checking::do_check( tag(), v );
	} 

}// container
}// checking
}// v2
}// assign
}// boost    
    
#endif
