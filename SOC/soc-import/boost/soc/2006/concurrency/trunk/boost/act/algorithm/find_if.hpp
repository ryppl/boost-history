//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_FIND_IF_HPP
#define BOOST_ACT_ALGORITHM_FIND_IF_HPP

#include "detail/make_algo.hpp"

#include "../act_fwd.hpp"

#include "find_if/detail/find_if_impl.hpp"

#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/identity.hpp>

#include <iterator>

#include "../detail/is_of_iterator_category.hpp"
#include "../type_traits/is_parallel_safe.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_MAKE_ALGO_IMPL( ((typename),IteratorType)
                                 ((typename),PredicateType)
                               , (IteratorType)
                               , find_if
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((PredicateType),predicate)
                               )
{
  typedef typename mpl::eval_if
          <
            is_parallel_safe< PredicateType >
          , mpl::if_
            <
              detail::is_of_iterator_category
              <
                IteratorType
              , ::std::random_access_iterator_tag
              >
            , find_if_safe_random_impl< AlgoModel >
            , find_if_safe_forward_impl< AlgoModel >
            >
          , mpl::identity< find_if_unsafe_impl< AlgoModel > >
          >
          ::type impl_type;

  return impl_type::execute( begin, end, predicate );
}

}
}

#include "../config/default_algo_model.hpp"

#endif
