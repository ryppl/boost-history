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
#include <boost/assign/v2/put/deduce/traits.hpp>
#include <boost/assign/v2/put/deduce/dependee.hpp>
#include <boost/assign/v2/put/container/functor/adapter.hpp>

namespace boost{
namespace assign{ 
namespace v2{
namespace result_of{

	template<typename C>
    struct put
    {
        typedef v2::put_traits<C> traits_;
        typedef typename traits_::functor_type f_;
        typedef typename traits_::modifier_tag modifier_tag_;
        typedef put_aux::adapter<C, f_, modifier_tag_> type;
    };

}// result_of

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

#endif
