//variadic template version of mpl/set.hpp
#ifndef BOOST_MPL_SET_HPP_INCLUDED
#define BOOST_MPL_SET_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/evansl/prog_dev/boost-svn/ro/boost-vrtmp/boost/mpl/RCS/set.hpp,v $
// $Date: 2009/08/25 20:17:50 $
// $Revision: 1.9 $

#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/set/set0.hpp>

namespace boost
{
namespace mpl
{

  template
  < class MemberKey
  , typename... MembersOther
  >
  struct
set
  < MemberKey
  , MembersOther...
  >
: s_item
  < MemberKey
  , typename set
    < MembersOther...
    >::item_
  >
//!
//!@nv-mpl_repl:
//!  This specialization corresponds to all of the setI
//!  for I=1..., which are defined in non-variadic mpl in 
//!  one of the files:
//!    boost/mpl/set/aux_/preprocessed/typeof_based/setN.hpp
//!  for some N in 10,20,...
//!
//!@nv-mpl_diff:
//!  WHAT:
//!    Instead of adding items to s_item from the tail, items are added from
//!    head.
//!  WHY:
//!    The variadic template compiler doesn't allow parameter
//!    packs to be followed by anything else (designated here as the
//!    [PACKS@END_CONSTRAINT]).  IOW:
//!      set<typename Head..., typename Tail>
//!    is not allowed because paramerter pack, Head..., is followed
//!    by non-pack Tail.
//!
{
        typedef 
      set
    type;
};

}//exit mpl namespace
}//exit boost namespace

#endif // BOOST_MPL_SET_HPP_INCLUDED
