//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_PUT_PIPE_MODULO_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_V2_PUT_PIPE_MODULO_TRAITS_ER_2010_HPP
#include <boost/mpl/apply.hpp>
#include <boost/mpl/size.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/vector/vector0.hpp>
#include <boost/assign/v2/ref/list/at.hpp>
#include <boost/assign/v2/ref/list/container.hpp>
#include <boost/assign/v2/ref/list/nth_result_of.hpp>
#include <boost/assign/v2/ref/list/make.hpp>

namespace boost{
    struct use_default;
namespace assign{
namespace v2{
namespace put_pipe_aux{

    typedef ::boost::mpl::vector0<> empty_pars_;

    template<typename Pars>
    struct modulo_traits
    {
        typedef ref::nth_result_of::list<use_default> meta_;
        typedef typename ::boost::mpl::apply1<meta_, Pars>::type cont_;
        typedef typename ::boost::mpl::size<Pars>::type size;

        template<typename T>
        struct new_pars : ::boost::mpl::push_back<Pars, T const&>{};

    };

}// put_pipe_aux
}// v2
}// assign
}// boost

#endif
