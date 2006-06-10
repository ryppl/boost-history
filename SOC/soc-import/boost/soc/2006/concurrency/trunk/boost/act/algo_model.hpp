//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_DETAIL_ALGO_MODEL_HPP
#define BOOST_ACT_DETAIL_ALGO_MODEL_HPP

namespace boost
{
namespace act
{
namespace detail
{
  struct algo_model_base
  {
  };
}

template< typename AlgoModelTag >
struct algo_model
  : detail::algo_model_base
{
  typedef AlgoModelTag algo_model_tag;
};

}
}

#endif
