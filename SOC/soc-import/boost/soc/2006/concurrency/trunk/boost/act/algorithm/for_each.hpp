//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_FOR_EACH_HPP
#define BOOST_ACT_ALGORITHM_FOR_EACH_HPP

#include "detail/make_algo.hpp"

#include "../act_fwd.hpp"

#include "for_each/detail/for_each_impl.hpp"

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
                                 ((typename),FunctionType)
                               , (FunctionType)
                               , for_each
                               , ((IteratorType),begin)
                                 ((IteratorType),end)
                                 ((FunctionType),function)
                               )
{
  typedef typename mpl::eval_if
          <
            is_parallel_safe< FunctionType >
          , mpl::if_
            <
              detail::is_of_iterator_category
              <
                IteratorType
              , ::std::random_access_iterator_tag
              >
            , for_each_safe_random_impl< AlgoModel >
            , for_each_safe_forward_impl< AlgoModel >
            >
          , mpl::identity< for_each_unsafe_impl< AlgoModel > >
          >
          ::type impl_type;

  return impl_type::execute( begin, end, function );
}

}
}

#include "../config/default_algo_model.hpp"

#endif
