//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CONTAINER_PUT_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CONTAINER_PUT_ER_2010_HPP
#include <boost/assign/v2/put/container/fwd.hpp>
#include <boost/assign/v2/put/container/adapter.hpp>
#include <boost/assign/v2/put/deduce/fun.hpp>
#include <boost/assign/v2/put/deduce/modifier/tag.hpp>
#include <boost/assign/v2/put/deduce/modifier/dependee.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace result_of{

    template<typename C>
    struct put
    {
        typedef typename put_aux::deduce_fun<C>::type f_;
        typedef typename put_aux::deduce_modifier_tag<C>::type modifier_tag_;
        typedef put_aux::container_adapter<C, f_, modifier_tag_> type;
    };

}// result_of

    template<typename C>
    typename result_of::put<C>::type
    put( C& cont )
    {
        typedef typename result_of::put<C>::type result_;
        return result_( cont );
    }

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CONTAINER_FUNCTOR_RESULT_OF_ER_2010_HPP
