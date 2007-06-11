/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_NO_ARGUMENT_FWD_HPP
#define BOOST_ACT_CONSUMER_POLICY_NO_ARGUMENT_FWD_HPP

#include <boost/act/consumer_policy/default_normalization_info_fwd.hpp>

namespace boost { namespace act {

struct no_argument;

template<>
struct default_normalization_info
{
  struct eludible;

  struct normalizer
  {
    typedef void result_type;

    result_type operator ()() const {}
  };

  struct denormalizer
  {
    typedef void result_type;

    result_type operator ()() const {}
  };
};

} }

#endif
