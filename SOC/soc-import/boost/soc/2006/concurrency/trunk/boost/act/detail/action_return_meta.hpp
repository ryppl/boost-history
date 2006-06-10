//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ACTION_RETURN_META_HPP
#define BOOST_ACT_DETAIL_ACTION_RETURN_META_HPP

#include "../act_fwd.hpp"

namespace boost
{
namespace act
{
namespace detail
{

struct action_return_meta
{
  template< typename Type, typename ActModel >
  struct apply
  {
    typedef action< Type, ActModel > type;
  };
};

}
}
}

#endif
