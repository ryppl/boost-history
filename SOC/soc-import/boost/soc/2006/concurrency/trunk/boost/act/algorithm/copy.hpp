//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_ALGORITHM_COPY_HPP
#define BOOST_ACT_ALGORITHM_COPY_HPP

#include "detail/make_algo.hpp"

#include "copy/detail/copy_impl.hpp"

#include <boost/iterator/iterator_traits.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>

#include <iterator>

#include "../type_traits/has_parallel_safe_assign.hpp"
#include "../detail/is_of_iterator_category.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_MAKE_ALGO_IMPL( ((typename),SourceIteratorType)
                                 ((typename),TargetIteratorType)
                               , (TargetIteratorType)
                               , copy
                               , ((SourceIteratorType),source_begin)
                                 ((SourceIteratorType),source_end)
                                 ((TargetIteratorType),target_begin)
                               )
{
  typedef typename iterator_value< TargetIteratorType >::type target_value_type;

  typedef typename mpl::eval_if
          <
            has_parallel_safe_assign< target_value_type >
          , mpl::if_
            <
              mpl::and_
              <
                detail::is_of_iterator_category
                <
                  SourceIteratorType
                , ::std::random_access_iterator_tag
                >
              , detail::is_of_iterator_category
                <
                  TargetIteratorType
                , ::std::random_access_iterator_tag
                >
              >
            , copy_safe_random_impl< AlgoModel >
            , copy_safe_forward_impl< AlgoModel >
            >
          , mpl::identity< copy_unsafe_impl< AlgoModel > >
          >
          ::type impl_type;

  return impl_type::execute( source_begin, source_end, target_begin );
}

}
}

#include "../config/default_algo_model.hpp"

#endif
