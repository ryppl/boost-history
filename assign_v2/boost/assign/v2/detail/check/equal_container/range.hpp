//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_RANGE_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_RANGE_ER_2010_HPP
#include <boost/range/algorithm/equal.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/traits/container/is_range.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace check_aux{ 

    template<typename C, typename R>
    void equal_container(v2::container_tag::range, C const & cont, R const& r)
    {                                                                              
        BOOST_ASSIGN_V2_CHECK( boost::equal( cont, r ) ); 
    }                                           

}// check_aux
}// v2
}// assign
}// boost    
    
#endif
