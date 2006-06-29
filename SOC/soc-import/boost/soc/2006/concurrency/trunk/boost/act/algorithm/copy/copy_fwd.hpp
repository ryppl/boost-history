//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_COPY_COPY_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_COPY_COPY_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO( ((typename),SourceIteratorType)
                               ((typename),TargetIteratorType)
                             , (TargetIteratorType)
                             , copy
                             , ((SourceIteratorType),source_begin)
                               ((SourceIteratorType),source_end)
                               ((TargetIteratorType),target_begin)
                             )

template< typename AlgoModel >
struct copy_safe_random_impl;

template< typename AlgoModel >
struct copy_safe_forward_impl;

}
}

#endif
