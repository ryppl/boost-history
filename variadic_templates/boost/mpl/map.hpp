//variadic template version of mpl/map.hpp
#ifndef BOOST_MPL_MAP_HPP_INCLUDED
#define BOOST_MPL_MAP_HPP_INCLUDED

// Copyright Aleksey Gurtovoy 2000-2004
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/mpl for documentation.

// $Source: /home/evansl/prog_dev/boost-svn/ro/sandbox-rw/variadic_templates/boost/mpl/RCS/map.hpp,v $
// $Date: 2009/11/23 14:43:13 $
// $Revision: 1.7 $

#include <boost/mpl/map/map0.hpp>
#include <boost/mpl/fold_assoc_pack.hpp>

namespace boost
{
namespace mpl
{
  template
  < typename... Values
  >
  struct
map
  : fold_assoc_pack
    < assoc_right
    , m_item_fold
    , map0
    , Values...
    >::type
{
};

}//exit mpl namespace
}//exit boost namespace

#include <boost/mpl/map/map0.hpp>

#endif // BOOST_MPL_MAP_HPP_INCLUDED
