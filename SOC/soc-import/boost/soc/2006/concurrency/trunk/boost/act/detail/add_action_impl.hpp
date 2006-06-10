//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ADD_ACTION_IMPL_HPP
#define BOOST_ACT_DETAIL_ADD_ACTION_IMPL_HPP

#include <boost/type_traits/remove_cv.hpp>

#include "../type_traits/remove_act_qualifiers.hpp"

#include "action_type.hpp"
#include "give_cv_to.hpp"

namespace boost
{
namespace act
{
namespace detail
{

template< typename Type, typename ActModel >
struct add_action_impl
  : give_cv_to
    <
      Type
    , action_type
      <
        typename remove_act_qualifiers
          < typename ::boost::remove_cv< Type >::type >::type
      , ActModel
      >
    >
{
};

}
}
}
#endif
