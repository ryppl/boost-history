//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_AUX_LIST_CONTAINER_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_AUX_LIST_CONTAINER_ER_2010_HPP
#include <boost/assign/v2/ref/aux_/list/fwd.hpp> // consistency
#include <boost/assign/v2/ref/aux_/list/holder/tail.hpp>
#include <boost/assign/v2/ref/aux_/list/holder/head.hpp>
#include <boost/assign/v2/ref/aux_/list/policy.hpp>
#include <boost/call_traits.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    template<typename Tag, typename H, typename T>
    class container :
        public tail_holder<T>,
        public head_holder<H>,
        public ::boost::mpl::apply2<policy<Tag>, H, T>::type
    {

        typedef tail_holder<T> tail_holder_;
        typedef head_holder<H> head_holder_;

        public:

        container(){}

        template<typename H1>
        struct result{
            typedef container this_;
            typedef container<Tag, H1, this_> type;
        };

        explicit container(
            typename boost::call_traits<T>::param_type t,
            H h
        ):
            tail_holder_( t )
            , head_holder_( h )
        {}

        template<typename H1>
        typename result<H1&>::type
        operator()(H1& h)const{
            typedef typename result<H1&>::type result_;
            return result_( *this, h );
        }

        template<typename H1>
        typename result<H1 const&>::type
        operator()(H1 const& h)const{
            typedef typename result<H1 const&>::type result_;
            return result_( *this, h );
        }

    };

}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_AUX_LIST_CONTAINER_ER_2010_HPP
