//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_CHECKING_ARRAY_ER_2010_HPP
#define BOOST_ASSIGN_V2_CHECKING_ARRAY_ER_2010_HPP
#include <boost/assign/v2/detail/checking/check.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/traits/container/is_array.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace checking{ 

    template<typename V>
    void do_check(v2::container_tag::array, V const& ar1)
    {                                                                              
        BOOST_ASSIGN_V2_CHECK( !ar1.empty() );
        V ar = ar1;
        namespace ns = checking::constants;
        ns::do_check(
           ar[0], ar[1], ar[2], ar[3],
           ar[4], ar[5], ar[6], ar[7]
        );                           
    }                                           

}// checking
}// v2
}// assign
}// boost    
    
#endif
