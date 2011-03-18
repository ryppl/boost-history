//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_REF_LIST_ARRAY_POLICY_ER_2010_HPP
#define BOOST_ASSIGN_V2_REF_LIST_ARRAY_POLICY_ER_2010_HPP
#include <boost/assign/v2/ref/array/alloc/lazy.hpp>
#include <boost/assign/v2/ref/list/fwd.hpp>
#include <boost/assign/v2/ref/list/holder.hpp>
#include <boost/assign/v2/ref/list/policy.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/type_traits/remove_cv.hpp>

namespace boost{
namespace assign{
namespace v2{
namespace ref{
namespace list_aux{

    // -- Fits the (linked) list with an array-like interface

    typedef alloc_tag::lazy_alloc array_tag;

    // policy_helperX<> extracts from the list the value-type for the array

    template<
        typename U1 // old result
        , typename U2 // candidate
        , typename V1 = typename boost::remove_cv<U1>::type
        , typename V2 = typename boost::remove_cv<U2>::type
    >
    struct policy_helper1
    {
        // Un-defined bec. V1 and V2 must be identical
    };

    template<typename U1, typename U2, typename V>
    struct policy_helper1<U1, U2, V, V> : boost::mpl::eval_if<
        boost::is_const<U1>
        , ::boost::mpl::identity<U1>
        , ::boost::mpl::identity<U2>
    >{
        // If one const, always const.
    };

    template<typename U1 // old result
        , typename U2, typename L // container<Tag, U2, L>
    >
    struct policy_helper2
    {
        typedef typename policy_helper1<U1, U2>::type new_u1_;
        typedef typename head_value<L>::type prior_u2_;
        typedef typename tail_of<L>::type prior_l_;
        typedef typename policy_helper2<
            new_u1_, prior_u2_, prior_l_
        >::type type;
    };

    template<typename U1>
    struct policy_helper2<U1, void_, nil >
    {
        // Reached root
        typedef U1 type;
    };

    template<typename U2, typename L>
    struct policy_helper3 : policy_helper2<
        U2
        , typename head_value<L>::type
        , typename tail_of<L>::type
    >{};

    template<>
    struct policy_helper3<void_, nil >
    {
        // Empty container
        typedef void_ type;
    };

    template<>
    struct policy<array_tag>
    {

        template<typename H = void_, typename T = nil>
        struct apply
        {
            typedef alloc_tag::lazy_alloc tag_;
            typedef list_aux::container<tag_, H, T> derived_;
            typedef typename boost::remove_reference<H>::type u1_;
            typedef typename list_aux::policy_helper3<u1_, T>::type value_;
            typedef array_aux::lazy_alloc<
                tail_holder<T>::static_size::value
                ,value_
                ,derived_
            > type;
        };

    };

}// list_aux
}// ref
}// v2
}// assign
}// boost

#endif // BOOST_ASSIGN_V2_REF_LIST_ARRAY_POLICY_ER_2010_HPP
