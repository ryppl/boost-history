//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_DEDUCE_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_DEDUCE_TRAITS_ER_2010_HPP
#include <boost/assign/v2/detail/traits/container/value.hpp>
#include <boost/assign/v2/detail/traits/container/is_ptr_container.hpp>
#include <boost/assign/v2/detail/functor/constructor.hpp>
#include <boost/assign/v2/detail/functor/new.hpp>
#include <boost/assign/v2/put/deduce/modifier.hpp>

namespace boost{
namespace assign{
namespace v2{

    template<typename C>
    struct put_traits
    {
        typedef typename v2::container_traits::value<C>::type value_type;
        typedef typename ::boost::mpl::eval_if<
            v2::container_traits::is_ptr_container<C>,
            functor_aux::deduce_new_<C>,
            functor_aux::deduce_constructor<C>
        >::type functor_type;
        typedef typename put_aux::deduce_modifier_tag<C>::type modifier_tag;
    };

}// v2
}// assign
}// boost

#endif
