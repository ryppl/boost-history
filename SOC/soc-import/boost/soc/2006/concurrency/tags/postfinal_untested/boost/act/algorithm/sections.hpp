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

#ifndef BOOST_ACT_ALGORITHM_SECTIONS_HPP
#define BOOST_ACT_ALGORITHM_SECTIONS_HPP

#include "detail/make_algo.hpp"

#include "sections/sections_fwd.hpp"

#include "../config/max_sections.hpp"

#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/repeat_from_to.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/arithmetic/inc.hpp>
#include <boost/preprocessor/cat.hpp>

#define BOOST_ACT_DETAIL_SECTIONS_IMPL_DESCIPTION_Z( z, index, dummy )         \

#define BOOST_ACT_DETAIL_EXECUTE_SECTION_FUN( z, index, dummy )                \
BOOST_PP_CAT( fun, index )();

#define BOOST_ACT_DETAIL_SECTIONS_IMPL_Z( z, index, dummy )                    \
BOOST_ACT_DETAIL_IMPLEMENT_ALGO_OVER                                           \
(                                                                              \
  BOOST_PP_CAT( BOOST_PP_REPEAT_, z )( BOOST_PP_INC( index )                   \
                                     , BOOST_ACT_DETAIL_SECTIONS_T_PARAM       \
                                     , BOOST_PP_NIL                            \
                                     )                                         \
  , (void)                                                                     \
  , sections                                                                   \
  , BOOST_PP_CAT( BOOST_PP_REPEAT_, z )( BOOST_PP_INC( index )                 \
                                       , BOOST_ACT_DETAIL_SECTIONS_PARAM       \
                                       , BOOST_PP_NIL                          \
                                       )                                       \
)                                                                              \
{                                                                              \
  BOOST_PP_CAT( BOOST_PP_REPEAT_, z )( BOOST_PP_INC( index )                   \
                                     , BOOST_ACT_DETAIL_EXECUTE_SECTION_FUN    \
                                     , BOOST_PP_NIL                            \
                                     )                                         \
}

namespace boost
{
namespace act
{

BOOST_ACT_DETAIL_PREPARE_ALGO_IMPLEMENTATION
(
  sections
, BOOST_ACT_MAX_SECTIONS
, ( BOOST_PP_ENUM( BOOST_ACT_MAX_SECTIONS
                 , BOOST_ACT_DETAIL_SECTIONS_DESCRIPTION_Z
                 , BOOST_PP_NIL
                 )
  )
)

BOOST_PP_REPEAT( BOOST_ACT_MAX_SECTIONS
               , BOOST_ACT_DETAIL_SECTIONS_IMPL_Z
               , BOOST_PP_NIL
               )

}
}

#endif
