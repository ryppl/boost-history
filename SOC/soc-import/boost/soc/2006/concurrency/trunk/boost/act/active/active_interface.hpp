//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACTIVE_ACTIVE_INTERFACE_HPP
#define BOOST_ACT_ACTIVE_ACTIVE_INTERFACE_HPP

#include "../act_fwd.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename ChildType, typename ActModel, typename ReturnMeta >
struct active_details
{
  typedef ChildType child_type;
  typedef ActModel act_model;
  typedef ReturnMeta return_meta;
};

}

#define BOOST_ACT_ACTIVE_INTERFACE_SPEC( type )                                \
template< typename ActiveDetails >                                             \
class active_interface< type, ActiveDetails > 

template< typename Type, typename ActiveDetails >
class active_interface
{
};

}
}

#endif
