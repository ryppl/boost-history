
#ifndef BOOST_MPL_SET_AUX_ITERATOR_HPP_INCLUDED
#define BOOST_MPL_SET_AUX_ITERATOR_HPP_INCLUDED

// + file: boost/mpl/aux_/iterator.hpp
// + last modified: 03/may/03

// Copyright (c) 2002-03
// David Abrahams, Aleksey Gurtovoy
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

#include "boost/mpl/set/aux_/set0.hpp"
#include "boost/mpl/has_key.hpp"
#include "boost/mpl/iterator_tag.hpp"
#include "boost/mpl/next.hpp"
#include "boost/mpl/apply_if.hpp"
#include "boost/mpl/if.hpp"
#include "boost/mpl/identity.hpp"
#include "boost/mpl/aux_/config/ctps.hpp"

namespace boost { namespace mpl {

// used by 'set_iter_impl'
template< typename Set, typename Tail > struct set_iter;

template< typename Set, typename Tail > struct set_iter_impl
{
    typedef Tail                tail_;
    typedef fwd_iter_tag_       category;
    typedef typename Tail::item type;

    typedef typename apply_if< 
          has_key< Set,typename Tail::base::item >
        , identity< set_iter<Set,typename Tail::base> >
        , next< set_iter<Set,typename Tail::base> >
        >::type next;        
};

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)

template< typename Set >
struct set_end_iter
{
    typedef fwd_iter_tag_           category;
    typedef set_iter<Set,set0<> >   next;
};

template< typename Set, typename Tail > struct set_iter
    : if_< 
          is_same< Tail,set0<> >
        , set_end_iter<Set>
        , set_iter_impl<Set,Tail>
        >::type
{
};

#else

template< typename Set, typename Tail > struct set_iter
    : set_iter_impl<Set,Tail>
{
};

template< typename Set > struct set_iter<Set, set0<> >
{
    typedef fwd_iter_tag_   category;
    typedef set_iter        next;
};

#endif

}}

#endif // BOOST_MPL_SET_AUX_ITERATOR_HPP_INCLUDED
