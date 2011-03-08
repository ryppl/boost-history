//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_CONTAINER_FUNCTOR_MAKE_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_CONTAINER_FUNCTOR_MAKE_ER_2010_HPP
#include <boost/assign/v2/put/deduce/modifier_dependee.hpp>
#include <boost/assign/v2/put/container/functor/adapter.hpp>
#include <boost/assign/v2/put/container/functor/result_of.hpp>

namespace boost{
namespace assign{
namespace v2{

    template<typename C>
    typename result_of::put<C>::type
    put( C& v )
    {
        typedef typename result_of::put<C>::type result_;
        return result_( v );
    }

}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_PUT_CONTAINER_FUNCTOR_MAKE_ER_2010_HPP
