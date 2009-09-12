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
// $Date: 2009/08/30 12:18:00 $
// $Revision: 1.10 $

#include <boost/mpl/aux_/value_wknd.hpp>
#include <boost/mpl/set/set0.hpp>
#include <boost/mpl/foldr_pack.hpp>

namespace boost
{
namespace mpl
{

  template
  < typename... Values
  >
  struct
set
  : foldr_pack
    < set0
    , s_item_fold
    , Values...
    >::type
//!
//!@nv-mpl_repl:
//!  This specialization corresponds to all of the setI
//!  for I=1..., which are defined in non-variadic mpl in 
//!  one of the files:
//!    boost/mpl/set/aux_/preprocessed/typeof_based/setN.hpp
//!  for some N in 10,20,...
//!
//!@nv-mpl_diff:
//!  Similar to that of vector.hpp.
{
};

}//exit mpl namespace
}//exit boost namespace

#endif // BOOST_MPL_SET_HPP_INCLUDED
