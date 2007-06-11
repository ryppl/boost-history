//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

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

#include "copy/copy_fwd.hpp"

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_IMPLEMENT_ALGO( ((typename),SourceIteratorType)
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
