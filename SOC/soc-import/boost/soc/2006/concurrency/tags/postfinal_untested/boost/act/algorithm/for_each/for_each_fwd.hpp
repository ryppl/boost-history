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

#ifndef BOOST_ACT_ALGORITHM_ALGORITHM_FOR_EACH_FOR_EACH_FWD_HPP
#define BOOST_ACT_ALGORITHM_ALGORITHM_FOR_EACH_FOR_EACH_FWD_HPP

#include "../detail/make_algo.hpp"

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/enum.hpp>

#include <boost/act/config/num_ranges.hpp>

namespace boost
{
namespace act
{

#define BOOST_ACT_DETAIL_FOR_EACH_IT( z, curr_index, dummy )                   \
((typename),BOOST_PP_CAT(IteratorType,curr_index))

#define BOOST_ACT_DETAIL_FOR_EACH_TYPE( z, curr_index, dummy )                 \
((BOOST_PP_CAT(IteratorType,curr_index)),BOOST_PP_CAT(begin,curr_index))

#define BOOST_ACT_DETAIL_FOR_EACH_SIG( z, curr_num_ranges, dummy )             \
(                                                                              \
  BOOST_PP_CAT(BOOST_PP_REPEAT_,z)                                             \
    (curr_num_ranges,BOOST_ACT_DETAIL_FOR_EACH_TYPE,BOOST_PP_NIL)              \
  ((typename),FunctionType)                                                    \
, (FunctionType)                                                               \
, ((IteratorType0),begin0)                                                     \
  ((IteratorType0),end0)                                                       \
  BOOST_PP_REPEAT_FROM_TO_Z( z, 1, curr_num_ranges                             \
                           , BOOST_ACT_DETAIL_FOR_EACH_IT, BOOST_PP_NIL        \
                           )                                                   \
  ((FunctionType),function)                                                    \
)

#define BOOST_ACT_DETAIL_FOR_EACH_OVERLOADS()                                  \
( BOOST_PP_ENUM( BOOST_ACT_MAX_RANGES                                          \
               , BOOST_ACT_DETAIL_FOR_EACH_SIG, BOOST_PP_NIL                   \
               )                                                               \
)

BOOST_ACT_DETAIL_DECLARE_ALGO_OVERLOADS
(
  for_each
, BOOST_ACT_MAX_RANGES
, BOOST_ACT_DETAIL_FOR_EACH_OVERLOADS()
)

}
}

#endif
