//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ACTION_AS_REF_HPP
#define BOOST_ACT_ACTION_AS_REF_HPP

#include "detail/act_ref.hpp"

namespace boost
{
namespace act
{

detail::act_ref_type const as_ref;

detail::act_cref_type const as_cref;

}
}

#endif
