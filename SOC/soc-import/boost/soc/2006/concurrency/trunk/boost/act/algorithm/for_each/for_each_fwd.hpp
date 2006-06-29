//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_FOR_EACH_FOR_EACH_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_FOR_EACH_FOR_EACH_FWD_HPP

#include "../detail/make_algo.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_DECLARE_ALGO( ((typename),IteratorType)
                               ((typename),FunctionType)
                             , (FunctionType)
                             , for_each
                             , ((IteratorType),begin)
                               ((IteratorType),end)
                               ((FunctionType),function)
                             )

template< typename AlgoModel >
struct for_each_safe_random_impl;

template< typename AlgoModel >
struct for_each_safe_forward_impl;

}
}

#endif
