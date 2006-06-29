//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_FIND_IF_FIND_IF_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_FIND_IF_FIND_IF_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO( ((typename),IteratorType)
                               ((typename),PredicateType)
                             , (IteratorType)
                             , find_if
                             , ((IteratorType),begin)
                               ((IteratorType),end)
                               ((PredicateType),predicate)
                             )

template< typename AlgoModel >
struct find_if_safe_random_impl;

template< typename AlgoModel >
struct find_if_safe_forward_impl;

}
}

#endif
