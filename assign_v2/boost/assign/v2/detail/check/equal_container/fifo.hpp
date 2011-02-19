//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_FIFO_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_CHECK_EQUAL_CONTAINER_FIFO_ER_2010_HPP
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/detail/traits/container/is_fifo.hpp>
#include <boost/assign/v2/detail/check_equal_container/pp.hpp>

namespace boost{                                                               
namespace assign{ 
namespace v2{
namespace check_aux{ 

    template<typename C, typename R>
    void equal_container(v2::container_tag::fifo, C const & cont, R const& r)
    {                                                                              
        BOOST_ASSIGN_V2_CHECK_AUX_CONTAINER_SIZE
        BOOST_ASSIGN_V2_CHECK_AUX_CONTAINER_ELEM_AT
        C cpy = cont; 
        for(size_type i = 0; i < n1; i++)
        {
            BOOST_ASSIGN_V2_CHECK( cpy.front() == elem_at( i ) ); cpy.pop(); 
        }
        BOOST_ASSIGN_V2_CHECK( cpy.empty() ); // consistency check
    }                                           

}// check_aux
}// v2
}// assign
}// boost    
    
#endif
