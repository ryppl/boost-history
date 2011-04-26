//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_DETAIL_CHECK_FWD_EQUAL_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_DETAIL_CHECK_FWD_EQUAL_CONTAINER_ER_2010_HPP

namespace boost{
namespace assign{
namespace v2{
namespace check_aux{

    template<typename C, typename R>
    void equal_container(C & cont, R const& r);

    template<typename C, typename R>
    void equal_container(C const& cont, R const& r);

}// check_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_DETAIL_CHECK_FWD_EQUAL_CONTAINER_ER_2010_HPP
