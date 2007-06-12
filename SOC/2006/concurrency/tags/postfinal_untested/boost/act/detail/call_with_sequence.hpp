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

#ifndef BOOST_ACT_DETAIL_CALL_WITH_SEQUENCE_HPP
#define BOOST_ACT_DETAIL_CALL_WITH_SEQUENCE_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/fusion/sequence/intrinsic/size.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/size_t.hpp>

#include <boost/act/config/num_ranges.hpp>

namespace boost
{
namespace act
{
namespace detail
{

#define BOOST_ACT_DETAIL_FOR_EACH_IF_ELEM( z, curr_index, dummy )              \
*fusion::at_c< curr_index >( begin_iterators )

#define BOOST_ACT_DETAIL_EXECUTE_FOR_EACH_FUN( z, curr_seq_size, dummy )       \
template< typename ReturnType, typename SequenceType, typename Function >      \
typename enable_if                                                             \
<                                                                              \
  mpl::equal_to                                                                \
  <                                                                            \
    fusion::result_of::size< SequenceType >                                    \
  , mpl::size_t< curr_seq_size >                                               \
  >                                                                            \
, ReturnType                                                                   \
>                                                                              \
::type                                                                         \
call_with_sequence( Function& function                                         \
                  , SequenceType& sequence                                     \
                  )                                                            \
{                                                                              \
  return function( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                           \
                   (                                                           \
                     curr_seq_size                                             \
                   , BOOST_ACT_DETAIL_FOR_EACH_IF_ELEM                         \
                   , BOOST_PP_NIL                                              \
                   )                                                           \
                 );                                                            \
}                                                                              \
                                                                               \
template< typename SequenceType, typename Function >                           \
typename enable_if                                                             \
<                                                                              \
  mpl::equal_to                                                                \
  <                                                                            \
    fusion::result_of::size< SequenceType >                                    \
  , mpl::size_t< curr_seq_size >                                               \
  >                                                                            \
>                                                                              \
::type                                                                         \
call_with_sequence( Function& function                                         \
                  , SequenceType& sequence                                     \
                  )                                                            \
{                                                                              \
  function( BOOST_PP_CAT( BOOST_PP_ENUM_, z )                                  \
            (                                                                  \
              curr_seq_size                                                    \
            , BOOST_ACT_DETAIL_FOR_EACH_IF_ELEM                                \
            , BOOST_PP_NIL                                                     \
            )                                                                  \
          );                                                                   \
}

BOOST_PP_REPEAT( BOOST_ACT_MAX_RANGES
               , BOOST_ACT_DETAIL_EXECUTE_FOR_EACH_FUN
               , BOOST_PP_NIL
               )

}
}
}

#endif
