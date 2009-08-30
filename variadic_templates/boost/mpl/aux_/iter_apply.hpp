
#ifndef BOOST_MPL_ITER_APPLY_HPP_INCLUDED
#define BOOST_MPL_ITER_APPLY_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2002-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/evansl/prog_dev/boost-cvs/ro/boost/sandbox/variadic-templates/boost/mpl/aux_/RCS/iter_apply.hpp,v $
// $Date: 2007/03/08 15:21:49 $
// $Revision: 1.4 $
//ChangeLog:
//  2006-10-28.1455.CST:Larry Evans
//    WHAT:
//      1) restore the iter_applyn name from the iter_apply name.
//    WHY:
//      1) avoid compile error about previous def taking 2 instead of 3
//         args.
//  2006-10-27.2148.CST:Larry Evans
//    WHAT:
//      1) cp'ed from $BOOST_ROOT/boost/mpl/aux_/iter_apply.hpp
//      2) replaced all applyn with apply
//    WHY:
//      1-2) applyn has been replaced with veriadic apply.

#include <boost/mpl/apply.hpp>
#include <boost/mpl/deref.hpp>

namespace boost { namespace mpl { namespace aux {

template
    < typename F
    , typename Iterator
    >
struct iter_apply1
    : apply
      < F
      , typename deref<Iterator>::type 
      >
{
};

template
    < typename F
    , typename Iterator1
    , typename Iterator2
    >
struct iter_apply2
    : apply
      < F
      , typename deref<Iterator1>::type
      , typename deref<Iterator2>::type
      >
{
};

}}}

#endif // BOOST_MPL_ITER_APPLY_HPP_INCLUDED
