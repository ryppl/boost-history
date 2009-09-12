
#ifndef BOOST_MPL_LIST_AUX_NODE_HPP_INCLUDED
#define BOOST_MPL_LIST_AUX_NODE_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Id$
// $Date: 2009/09/01 19:28:35 $
// $Revision: 1.10 $

#include <boost/mpl/list/aux_/tag.hpp>
#include <boost/mpl/aux_/config/msvc.hpp>
#include <boost/mpl/aux_/config/workaround.hpp>

namespace boost { namespace mpl {

template<
      typename T
    , typename Next
    >
struct l_item
  : Next
//!  
//!@diff-nv-mpl:
//!  WHAT:
//!    Inherits from Next.
//!  WHY:
//!    Elimiates need to typedef tag (inherited from last Next)
//!    and, in the case of list_c, the need to typedef value_type
//!    (since it's also inherited from last Next, which, in the
//!    case of list_c, would be list0_c.
//!
{
// agurt, 17/jul/03: to facilitate the deficient 'is_sequence' implementation 
#if BOOST_WORKAROUND(BOOST_MSVC, <= 1300)
    typedef int begin;
#endif
    typedef l_item type;

    typedef typename next<typename Next::size>::type size;
    typedef T item;
    typedef Next next;
};

template<
      typename T
    , typename Next
    >
struct l_item_fold
{
    typedef l_item<T,Next> type;
};

}}

#endif // BOOST_MPL_LIST_AUX_NODE_HPP_INCLUDED
