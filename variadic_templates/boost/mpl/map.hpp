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

// $Source: /home/evansl/prog_dev/boost-svn/ro/boost-vrtmp/boost/mpl/RCS/map.hpp,v $
// $Date: 2009/09/02 18:45:20 $
// $Revision: 1.5 $

#include <boost/mpl/map/map0.hpp>
#include <boost/mpl/foldr_pack.hpp>

namespace boost
{
namespace mpl
{
  template
  < typename... Values
  >
  struct
map
  : foldr_pack
    < map0
    , m_item_fold
    , Values...
    >::type
{
};

}//exit mpl namespace
}//exit boost namespace

#include <boost/mpl/map/map0.hpp>

#endif // BOOST_MPL_MAP_HPP_INCLUDED
