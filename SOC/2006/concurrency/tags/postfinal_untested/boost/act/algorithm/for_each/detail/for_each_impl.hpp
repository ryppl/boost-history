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

#ifndef BOOST_ACT_ALGORITHM_DETAIL_FOR_EACH_IMPL_HPP
#define BOOST_ACT_ALGORITHM_DETAIL_FOR_EACH_IMPL_HPP

#include "for_each_basic.hpp"

#include <algorithm>

#include "../../../act_fwd.hpp"

#include "../../basic_for.hpp"

namespace boost
{
namespace act
{

// ToDo: Fix implementation
template< typename AlgoModel >
struct for_each_random_impl
{
  template< typename ExtendedParams
          , typename IteratorType
          , typename FunctionType
          >
  static FunctionType execute( ExtendedParams const& extended_params
                             , IteratorType begin
                             , IteratorType end
                             , FunctionType const& function
                             )
  {/*
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
*/
    return detail::for_each_basic( extended_params, begin, end, function );
  }
};

template< typename AlgoModel >
struct for_each_forward_impl
{
  template< typename ExtendedParams
          , typename IteratorType
          , typename FunctionType
          >
  static FunctionType execute( ExtendedParams const& extended_params
                             , IteratorType begin
                             , IteratorType end
                             , FunctionType const& function
                             )
  {
    // ToDo: Possibly change to spawn active objects which queue functions
    return ::std::for_each( begin, end, function );
  }
};

}
}

#endif
