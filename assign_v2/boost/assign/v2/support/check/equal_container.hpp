//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2011 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_SUPPORT_CHECK_EQUAL_CONTAINER_ER_2011_HPP
#define BOOST_ASSIGN_V2_SUPPORT_CHECK_EQUAL_CONTAINER_ER_2011_HPP
#include <boost/assign/v2/support/check/equal_container/array.hpp>
#include <boost/assign/v2/support/check/equal_container/deduce.hpp>
#include <boost/assign/v2/support/check/equal_container/fifo.hpp>
#include <boost/assign/v2/support/check/equal_container/lifo.hpp>
#include <boost/assign/v2/support/check/equal_container/range.hpp>
#include <boost/assign/v2/support/check/equal_container/sorted.hpp>
#include <boost/assign/v2/support/check/fwd_equal_container.hpp> // consistency

namespace boost{
namespace assign{
namespace v2{
namespace check_aux{

    template<typename C, typename R>
    void equal_container(C & cont, R const& r)
    {
        typedef typename v2::check_aux::deduce_equal_container_tag<C>::type tag;
        v2::check_aux::equal_container( tag(), cont, r );
    }

    template<typename C, typename R>
    void equal_container(C const& cont, R const& r)
    {
        typedef typename v2::check_aux::deduce_equal_container_tag<C>::type tag;
        v2::check_aux::equal_container( tag(), cont, r );
    }

}// check_aux
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_SUPPORT_CHECK_EQUAL_CONTAINER_ER_2011_HPP
