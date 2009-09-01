
#ifndef BOOST_MPL_VECTOR_AUX_ITEM_HPP_INCLUDED
#define BOOST_MPL_VECTOR_AUX_ITEM_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/03/27 20:36:14 $
// $Revision: 1.3 $

#include <boost/mpl/long.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/aux_/type_wrapper.hpp>
#include <boost/mpl/aux_/config/typeof.hpp>
#include <boost/mpl/aux_/config/ctps.hpp>

namespace boost { namespace mpl {

template
    < typename T
    , typename Base //vector<Tail...> for some Tail...
    , int at_front=1
    >
struct v_item
//!@nv-mpl_diff:
//!  WHAT:
//!    Default for at_front=1 instead of 0.
//!  WHY:
//!    The non-variadic vectorN<T1,...,TN>,
//!    enters T values into v_iterm from TN to T1,
//!    i.e. from back to front.
//!
//!    The variadic compiler disallows parameter packs
//!    at the beginning; hence, the last element, e.g.
//!    TN, can't be selected in a specialization such
//!    as vector<T...,TN>:v_item<TN,vector<T...> >.
//!    
//!    The workaround is to make this change.
//!  CONSEQUENCES:
//!    1) push_front cannot be called as many
//!       times.
//!    2) push_back can be called more times.
;
template
    < typename T
    , typename Base
    >
struct v_item<T,Base,0> //!extend Base at back with T.
    : Base
{
    typedef typename Base::upper_bound_ index_;
    typedef typename next<index_>::type upper_bound_;
    typedef typename next<typename Base::size>::type size;
    typedef Base base;
    typedef v_item type;

    // agurt 10/sep/04: MWCW <= 9.3 workaround here and below; the compiler
    // breaks if using declaration comes _before_ the new overload
    static aux::type_wrapper<T> item_(index_);
    using Base::item_;
};

template
    < typename T
    , typename Base
    >
struct v_item<T,Base,1> //extend Base at front with T.
    : Base
{
    typedef typename prior<typename Base::lower_bound_>::type index_;
    typedef index_ lower_bound_;
    typedef typename next<typename Base::size>::type size;
    typedef Base base;
    typedef v_item type;

    static aux::type_wrapper<T> item_(index_);
    using Base::item_;
};

template
    < typename T
    , typename Base
    >
struct v_item_fold
    : v_item<T,Base,1>
{
};

// "erasure" item
template< 
      typename Base
    , int at_front
    >
struct v_mask
    : Base
{
    typedef typename prior<typename Base::upper_bound_>::type index_;
    typedef index_ upper_bound_;
    typedef typename prior<typename Base::size>::type size;
    typedef Base base;
    typedef v_mask type;

    static aux::type_wrapper<void_> item_(index_);
    using Base::item_;
};

template< 
      typename Base
    >
struct v_mask<Base,1>
    : Base
{
    typedef typename Base::lower_bound_ index_;
    typedef typename next<index_>::type lower_bound_;
    typedef typename prior<typename Base::size>::type size;
    typedef Base base;
    typedef v_mask type;

    static aux::type_wrapper<void_> item_(index_);
    using Base::item_;
};

}}

#endif // BOOST_MPL_VECTOR_AUX_ITEM_HPP_INCLUDED
